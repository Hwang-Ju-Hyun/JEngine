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

Bomb::Bomb(GameObject* _owner)
	:BaseComponent(_owner)
{	
	m_pCol = static_cast<Collision*>(_owner->AddComponent(Collision::CollisionTypeName,new Collision(_owner)));
	m_pCurrentLevel = GameStateManager::GetInstance()->GetCurrentLevel();	
	CollisionManager::GetInstance()->AddBombToBombColVec(m_pCol);
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
	static float remain_explode_AccTime = 0.f;
	float dt = TimeManager::GetInstance()->GetDeltaTime();

	remain_explode_AccTime += dt;
	Sprite* bomb_spr = static_cast<Sprite*>(m_pOwner->FindComponent(Sprite::SpriteTypeName));

	int wall_width = TileEditor::GetInstance()->GetWallWidth();
	int wall_height = TileEditor::GetInstance()->GetWallHeight();

	static float ExplodingAccTime = 0.f;
	static int bomb_frag_cnt = 0;
	bomb_frag_cnt = 0;

	if (remain_explode_AccTime >= m_fRemaingTime)
	{				
		SetIsExplode(true);
		remain_explode_AccTime = 0.f;								
	}
	if (GetIsExplode())
	{
		ExplodingAccTime += dt;		
		bomb_spr->SetColor({ 1.0f, 0.f, 0.f, 1.f });
		//동북서남 반시계방향
		//						  →     ↑     ←       ↓
		int direction[4][2] = { {1,0},{0,-1},{-1,0},{0,1} };
		Transform* bomb_trs = static_cast<Transform*>(m_pOwner->FindComponent(Transform::TransformTypeName));
		glm::vec2 cur_bomb_grid = bomb_trs->GetGridByScreenPos();
		std::cout << std::endl;
		std::cout <<"Bomb Grid : "<< cur_bomb_grid.x << "," << cur_bomb_grid.y << std::endl;

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
				if (nextX >= screen_grid.size() && nextY >= screen_grid[0].size())
					continue;

				GameObject* wall_fragile_obj = TileEditor::GetInstance()->FindObjectByGrid({ nextX, nextY });
				if (wall_fragile_obj != nullptr)
				{
					Wall* wall_fragile_comp = static_cast<Wall*>(wall_fragile_obj->FindComponent(Wall::WallTypeName));
					if (wall_fragile_comp!=nullptr&&wall_fragile_comp->GetFragile())
					{
						m_vecBombFragment.push_back(Prefabs::GetInstance()->CreateBombs("json/Bomb/Bomb.json", this->GetOwner()));
						Transform* bomb_frag_trs = static_cast<Transform*>(m_vecBombFragment[bomb_frag_cnt]->GetOwner()->FindComponent(Transform::TransformTypeName));
						bomb_frag_trs->SetPosition(TileEditor::GetInstance()->GetWorldPosbyScreenGrid(wall_width, wall_height, nextX, nextY));
						Sprite* bomb_frag_spr = static_cast<Sprite*>(m_vecBombFragment[bomb_frag_cnt]->GetOwner()->FindComponent(Sprite::SpriteTypeName));
						bomb_frag_spr->SetColor({ 1.f,1.f,0.f,1.f });
						bomb_frag_cnt++;
					}
				}
				if (wall_fragile_obj == nullptr)
				{
					m_vecBombFragment.push_back(Prefabs::GetInstance()->CreateBombs("json/Bomb/Bomb.json", this->GetOwner()));
					Transform* bomb_frag_trs = static_cast<Transform*>(m_vecBombFragment[bomb_frag_cnt]->GetOwner()->FindComponent(Transform::TransformTypeName));
					bomb_frag_trs->SetPosition(TileEditor::GetInstance()->GetWorldPosbyScreenGrid(wall_width, wall_height, nextX, nextY));
					Sprite* bomb_frag_spr = static_cast<Sprite*>(m_vecBombFragment[bomb_frag_cnt]->GetOwner()->FindComponent(Sprite::SpriteTypeName));
					bomb_frag_spr->SetColor({ 1.f,1.f,0.f,1.f });
					bomb_frag_cnt++;
				}

				//벽이 폭탄에 맞았다면
				if (TileEditor::GetInstance()->m_vecWallGridCoord[nextX][nextY])
				{
					GameObject* wall_obj = TileEditor::GetInstance()->FindObjectByGrid({ nextX, nextY });
					if (wall_obj != nullptr)
					{
						Transform* wall_trs = static_cast<Transform*>(wall_obj->FindComponent(Transform::TransformTypeName));
						Wall* wall_comp = dynamic_cast<Wall*>(wall_obj->FindComponent(Wall::WallTypeName));
						if (wall_comp != nullptr)
						{
							if (wall_comp->GetFragile())
							{
								Collision* wall_col = dynamic_cast<Collision*>(wall_obj->FindComponent(Collision::CollisionTypeName));
								if (wall_comp != nullptr)
								{
									wall_comp->SetExist(false);
								}
								TileEditor::GetInstance()->m_vecWallGridCoord[nextX][nextY] = false;
								CollisionManager::GetInstance()->RemoveWallCol(wall_col);
								GameObjectManager::GetInstance()->RemoveObject(wall_obj->GetID(), Wall::WallTypeName);
								std::string cur_level_str = m_pCurrentLevel->GetName();
								Serializer::GetInstance()->SaveScreenGrid("json/" + cur_level_str + "/" + "Grid" + ".json");
							}
						}
					}
				}
				std::vector<Player*> players = GameObjectManager::GetInstance()->GetPlayerCompVec();
				for (int i = 0; i < players.size(); i++)
				{
					Transform* player_trs = static_cast<Transform*>(players[i]->GetOwner()->FindComponent(Transform::TransformTypeName));
					glm::vec2 player_grid = player_trs->GetGridByScreenPos();
					if (player_grid.x == nextX && player_grid.y == nextY)
					{
						//std::cout << players[i]->GetOwner()->GetID() << "is lose" << std::endl;						
					}
				}
			}
		}		
	}
	if (ExplodingAccTime >= GetExplodingTime())
	{
		for (int i = 0; i < m_vecBombFragment.size(); i++)
		{
			CollisionManager::GetInstance()->RemoveBombCol(static_cast<Collision*>(m_vecBombFragment[i]->GetOwner()->FindComponent(Collision::CollisionTypeName)));
			GameObjectManager::GetInstance()->RemoveObject(m_vecBombFragment[i]->GetOwner());
		}

		ExplodingAccTime = 0.f;
		m_vecBombFragment.clear();
		SetIsExplode(false);
		bomb_frag_cnt = 0;
		CollisionManager::GetInstance()->RemoveBombCol(m_pCol);
		GameObjectManager::GetInstance()->RemoveObject(m_pOwner->GetID(), Bomb::BombTypeName);
		remain_explode_AccTime = 0.f;
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
	}
}

json Bomb::SaveToJson(const json& _str)
{
	json data;

	data["Type"] = "Bomb";

	json comp_data;
	comp_data["Remain_Time"] = GetRemainTime();
	comp_data["Exploding_Time"] = GetExplodingTime();
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
