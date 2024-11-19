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
#include <iostream>

float Bomb::AccFragmentExplodeTime = 0.f;

Bomb::Bomb(GameObject* _owner)
	:BaseComponent(_owner)
{	
	m_pCol = static_cast<Collision*>(_owner->AddComponent(Collision::CollisionTypeName,new Collision(_owner)));
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

void Bomb::Update()
{	
	static float AccTime = 0.f;
	float dt = TimeManager::GetInstance()->GetDeltaTime();
	AccTime += dt;
	Sprite* bomb_spr = static_cast<Sprite*>(m_pOwner->FindComponent(Sprite::SpriteTypeName));
	auto a1 = GameObjectManager::GetInstance()->GetAllObject();
	auto c1 = ComponentManager::GetInstance()->GetAllComponents();
	a1, c1;
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
		}
		if(flag)
			AccFragmentExplodeTime += dt;		

		AccTime = 0.f;
		//CollisionManager::GetInstance()->RemoveBombCol(m_pCol);
		//GameObjectManager::GetInstance()->RemoveObject(m_pOwner->GetID(), Bomb::BombTypeName);
	}
	auto a = GameObjectManager::GetInstance()->GetAllObject();
	auto c = ComponentManager::GetInstance()->GetAllComponents();
	a, c;
	if (AccFragmentExplodeTime >= m_fBombFragExplodeTime)
	{
		SetIsExplode(true);
		flag = false;
		for (int i = 0; i < m_vecBombFragment.size(); i++)
		{
			GameObjectManager::GetInstance()->RemoveObject(m_vecBombFragment[i]);
		}
		m_vecBombFragment.clear();
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

	//Todo: 함수화 시키자
	std::vector<std::vector<bool>>& screen_grid = TileEditor::GetInstance()->GetWallGrid();
	for (int i = 0; i < 4; i++)
	{
		for (int range = 1; range <= m_iBombRange; range++)
		{			
			int nextX = direction[i][0] + (int)cur_bomb_grid.x * range;
			int nextY = direction[i][1] + (int)cur_bomb_grid.y * range;

			if (nextX < 0 || nextY < 0)
				continue;
			if (nextX >= screen_grid.size() || nextY >= screen_grid[0].size())
				continue;
			if (TileEditor::GetInstance()->m_vecWallGridCoord[nextX][nextY])			//벽에 맞았으면 생성 x
				continue;

			Bomb* bomb_frag = Prefabs::GetInstance()->CreateBombs("json/Bomb/Bomb.json", _bomb->GetOwner());
			m_vecBombFragment.push_back(bomb_frag->m_pOwner);
			bomb_frag->SetIsFragment(true);
			bomb_frag->SetRemainTime(0.f);			
			Transform* bomb_frag_trs = static_cast<Transform*>(bomb_frag->GetOwner()->FindComponent(Transform::TransformTypeName));			
			bomb_frag_trs->SetPosition(bomb_trs->GetPosition());
			Sprite* bomb_frag_spr= static_cast<Sprite*>(bomb_frag->GetOwner()->FindComponent(Sprite::SpriteTypeName));
			if (i == 1)
				direction[i][1] *= -1;
			bomb_frag_trs->AddPosition({ bomb_scale.x * range * direction[i][0],bomb_scale.y * range * direction[i][1] });
			auto b = bomb_pos;
			b;
			auto a= bomb_frag_trs->GetPosition();
			a;
			bomb_frag_spr->SetColor({ 1.0f,0.f,0.f,1.f });
		}
	}		
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
