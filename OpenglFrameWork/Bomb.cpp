#include "Bomb.h"
#include "GameObject.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "TileEditor.h"
#include "Sprite.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collision.h"
#include "TimeManager.h"
#include "Serializer.h"
#include "GameStateManager.h"
#include "BaseLevel.h"
#include "ComponentManager.h"
#include "Wall.h"
#include "Prefabs.h"
#include "BombManager.h"
#include <iostream>

float Bomb::AccFragmentExplodeTime = 0.f;
bool Bomb::flag = false;

Bomb::Bomb(GameObject* _owner)
	:BaseComponent(_owner)
{
	m_pCol = static_cast<Collision*>(_owner->AddComponent(Collision::CollisionTypeName, new Collision(_owner)));
	m_pCurrentLevel = GameStateManager::GetInstance()->GetCurrentLevel();
	CollisionManager::GetInstance()->AddBombToBombColVec(m_pCol);
	m_fBombFragExplodeTime = 3.f;
}

Bomb::~Bomb()
{
}

void Bomb::SetBombRange(int _range)
{
	m_iBombRange = _range;
}

int Bomb::GetBombRange() const
{
	return m_iBombRange;
}

void Bomb::SetIsExplode(bool _explode)
{
	m_bIsExplode = _explode;
}

bool Bomb::GetIsExplode() const
{
	return m_bIsExplode;
}

void Bomb::SetIsFragment(bool _frag)
{
	m_bIsFragmenet = _frag;
}

bool Bomb::GetIsFragment() const
{
	return m_bIsFragmenet;
}

void Bomb::SetRemainTime(float _time)
{
	m_fRemaingTime = _time;
}

void Bomb::AddRemainTime(float _time)
{
	m_fRemaingTime += _time;
}

const float Bomb::GetRemainTime() const
{
	return m_fRemaingTime;
}

void Bomb::SetExplodingTime(float _time)
{
	m_fExplodingTime = _time;
}

const float Bomb::GetExplodingTime() const
{
	return m_fExplodingTime;
}
static int a = 1;
void Bomb::Update()
{	
	static float AccTime = 0.f;
	float dt = TimeManager::GetInstance()->GetDeltaTime();
	AccTime += dt;
	Sprite* bomb_spr = static_cast<Sprite*>(m_pOwner->FindComponent(Sprite::SpriteTypeName));
	static int cnt = 0;
	if (AccTime >= m_fRemaingTime)
	{
		bomb_spr->SetColor({ 1.0f, 0.f, 0.f, 1.f });

		//동북서남 반시계방향
		int direction[4][2] = { {1,0},{0,-1},{-1,0},{0,1} };
		Transform* bomb_trs = static_cast<Transform*>(m_pOwner->FindComponent(Transform::TransformTypeName));

		glm::vec2 cur_bomb_grid = bomb_trs->GetGridByScreenPos();

		if (!GetIsExplode()&&!flag)
		{
			CreateBombFragment(static_cast<Bomb*>(GetOwner()->FindComponent(Bomb::BombTypeName)));
			flag = true;
			std::cout << cnt++ << std::endl; a++;
		}
		

		if (flag)
		{
			float frag_explode_dt = TimeManager::GetInstance()->GetDeltaTime();
			AccFragmentExplodeTime += frag_explode_dt;
		}

		AccTime = 0.f;
		//CollisionManager::GetInstance()->RemoveBombCol(m_pCol);
		//GameObjectManager::GetInstance()->RemoveObject(m_pOwner->GetID(), Bomb::BombTypeName);
	}

	if (AccFragmentExplodeTime >= m_fBombFragExplodeTime&&flag)
	{
		SetIsExplode(true);
		flag = false;
		auto all_bomb_fragments = BombManager::GetInstance()->GetAllBombFragmenets();
		for (int i = 0; i < all_bomb_fragments.size(); i++)
		{
			Collision* bomb_frag_col = static_cast<Collision*>(all_bomb_fragments[i]->FindComponent(Collision::CollisionTypeName));
			CollisionManager::GetInstance()->RemoveBombCol(bomb_frag_col);
			GameObjectManager::GetInstance()->RemoveObject(all_bomb_fragments[i]);
			
			all_bomb_fragments[i] = nullptr;
		}
		all_bomb_fragments.clear();
		AccFragmentExplodeTime = 0.f;		
	}

}

void Bomb::CreateBombFragment(Bomb* _bomb)
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();

	//						  →     ↑      ←      ↓
	int direction[4][2] = { {1,0},{0,-1},{-1,0},{0,1} };

	Transform* bomb_trs = static_cast<Transform*>(_bomb->GetOwner()->FindComponent(Transform::TransformTypeName));
	glm::vec2 cur_bomb_grid = bomb_trs->GetGridByScreenPos();
	glm::vec2 bomb_pos = bomb_trs->GetPosition();
	glm::vec2 bomb_scale = bomb_trs->GetScale();	

	//Todo: 개 미 친 놈 코드 <- 무조건 수정 해야 함 
	std::vector<std::vector<bool>>& screen_grid = TileEditor::GetInstance()->GetWallGrid();
	for (int i = 0; i < 4; i++)
	{
		for (int range = 0; range < m_iBombRange; range++)
		{		
			int nextX;
			int nextY;
			if (i % 2 == 0)
			{				
				if (i == 2)
					range *= -1;
				nextX = direction[i][0] + (int)cur_bomb_grid.x + range;
				nextY = direction[i][1] + (int)cur_bomb_grid.y;
			}			
			else
			{
				if (i == 1)
				{					
					range *= -1;
				}					
				nextX = direction[i][0] + (int)cur_bomb_grid.x ;
				nextY = direction[i][1] + (int)cur_bomb_grid.y + range;				
			}

			if (nextX < 0 || nextY < 0)
				continue;
			if (nextX >= screen_grid.size() || nextY >= screen_grid[0].size())
				continue;
			if (TileEditor::GetInstance()->m_vecWallGridCoord[nextX][nextY])			//벽에 맞았으면 생성 x
			{
				GameObject* wall_obj = TileEditor::GetInstance()->FindObjectByGrid({ nextX, nextY });
				if (wall_obj != nullptr)
				{					
					Wall* wall_comp = dynamic_cast<Wall*>(wall_obj->FindComponent(Wall::WallTypeName));
					if (wall_comp != nullptr)
					{
						break;
					}
				}
			}

			Bomb* bomb_frag = Prefabs::GetInstance()->CreateBombs("json/Bomb/Bomb.json", _bomb->GetOwner());			
			bomb_frag->SetIsFragment(true);
			bomb_frag->SetRemainTime(0.f);
			Transform* bomb_frag_trs = static_cast<Transform*>(bomb_frag->GetOwner()->FindComponent(Transform::TransformTypeName));
			bomb_frag_trs->SetPosition(bomb_trs->GetPosition());
			Sprite* bomb_frag_spr = static_cast<Sprite*>(bomb_frag->GetOwner()->FindComponent(Sprite::SpriteTypeName));
			
			if (i == 1)
			{
				bomb_frag_trs->SetPosition({ bomb_trs->GetPosition().x,bomb_trs->GetPosition().y + (bomb_scale.y * (range - 1) * direction[i][1]) });
				range *= -1;
			}
			else if (i == 2)
			{				
				direction[i][0] *= -1;			
				bomb_frag_trs->SetPosition({ bomb_trs->GetPosition().x + (bomb_scale.x * (range - 1) * direction[i][0]), bomb_trs->GetPosition().y });
				range *= -1;				
				direction[i][0] *= -1;				
			}
			else
			{				
				direction[i][1] *= -1;			
				bomb_frag_trs->SetPosition({ bomb_trs->GetPosition().x + (bomb_scale.x * (range + 1) * direction[i][0]),bomb_trs->GetPosition().y + (bomb_scale.y * (range + 1) * direction[i][1]) });				
				direction[i][1] *= -1;				
			}
													
			bomb_frag_spr->SetColor({ 1.0f,0.f,0.f,1.f });			
			BombManager::GetInstance()->AddBombFragment(bomb_frag->m_pOwner);
			//m_vecBombFragment.push_back(bomb_frag->m_pOwner);
		}
	}	
	auto vec=BombManager::GetInstance()->GetAllBombFragmenets();
	vec;
	int a = 0;
}


void Bomb::EventCollision(Collision* _pOther)
{
	if (_pOther->GetOwner()->GetName() == Wall::WallTypeName)
	{
		CollisionManager::GetInstance()->HandlePosOnCollision_Rect_Rect(_pOther->GetOwner(), m_pOwner);
	}
	if (_pOther->GetOwner()->GetName() == Bomb::BombTypeName)
	{
		CollisionManager::GetInstance()->HandlePosOnCollision_Rect_Rect(_pOther->GetOwner(), m_pOwner);
	}
}

void Bomb::LoadFromJson(const json& _str)
{
	auto comp_data = _str.find("CompData");
	if (comp_data != _str.end())
	{
		auto remain_time = comp_data->find("Remain_Time");
		m_fRemaingTime = remain_time->begin().value();
		auto explode_time = comp_data->find("Exploding_Time");
		m_fExplodingTime = explode_time->begin().value();
		auto is_fragment = comp_data->find("IsFragment");
		m_bIsFragmenet = is_fragment->begin().value();
	}
}

json Bomb::SaveToJson(const json& _str)
{
	json data;

	data["Type"] = "Bomb";

	json comp_data;
	comp_data["Remain_Time"] = GetRemainTime();
	comp_data["Exploding_Time"] = GetExplodingTime();
	comp_data["IsFragment"] = GetIsFragment();
	data["CompData"] = comp_data;


	return data;
}

BaseRTTI* Bomb::CreateBombComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent(Bomb::BombTypeName, new Bomb(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - Bomb::CreateBombComponent" << std::endl;
		return nullptr;
	}
	return p;
}