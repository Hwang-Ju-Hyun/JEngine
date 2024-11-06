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
#include "Wall.h"
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

void Bomb::SetExplodeTime(float _time)
{
	m_fExplodeTime = _time;
}

void Bomb::AddExplodeTime(float _time)
{
	m_fExplodeTime += _time;
}

const float Bomb::GetExplodeTime() const
{
	return m_fExplodeTime;
}

void Bomb::Update()
{
	static float AccTime = 0.f;
	float dt = TimeManager::GetInstance()->GetDeltaTime();
	AccTime += dt;
	Sprite* bomb_spr = static_cast<Sprite*>(m_pOwner->FindComponent(Sprite::SpriteTypeName));
	if (AccTime >= m_fExplodeTime)
	{
		SetIsExplode(true);
		bomb_spr->SetColor({ 1.0f, 0.f, 0.f, 1.f });
		//동남서북 반시계방향
		int direction[4][2] = { {1,0},{0,-1},{-1,0},{0,1} };
		Transform* bomb_trs = static_cast<Transform*>(m_pOwner->FindComponent(Transform::TransformTypeName));

		glm::vec2 cur_bomb_grid = bomb_trs->GetGridByScreenPos();

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

				if (TileEditor::GetInstance()->m_vecWallGridCoord[nextX][nextY])
				{
					GameObject* wall_obj = TileEditor::GetInstance()->FindObjectByGrid({ nextX, nextY });
					Transform* wall_trs = static_cast<Transform*>(wall_obj->FindComponent(Transform::TransformTypeName));
					Wall* wall_comp = dynamic_cast<Wall*>(wall_obj->FindComponent(Wall::WallTypeName));
					if (wall_comp != nullptr)
					{
						wall_comp->SetExist(false);
					}
					TileEditor::GetInstance()->m_vecWallGridCoord[nextX][nextY] = false;
					GameObjectManager::GetInstance()->RemoveObject(wall_obj->GetID(), Wall::WallTypeName);

					std::string cur_level_str = m_pCurrentLevel->GetName();
					Serializer::GetInstance()->SaveScreenGrid("json/" + cur_level_str + "/" + "Grid" + ".json");
				}
			}
		}		
		AccTime = 0.f;
		GameObjectManager::GetInstance()->RemoveObject(m_pOwner->GetID(), Bomb::BombTypeName);
	}

	


	
	
}

void Bomb::EventCollision(Collision* _pOther)
{
	if (_pOther->GetOwner()->GetName() == Wall::WallTypeName)
	{
		CollisionManager::GetInstance()->HandlePosOnCollision_Rect_Rect(_pOther->GetOwner(), m_pOwner);

	}
		
}

void Bomb::LoadFromJson(const json& _str)
{
	auto comp_data = _str.find("CompData");
	if (comp_data != _str.end())
	{
		auto explode_time = comp_data->find("Explode_Time");
		m_fExplodeTime = explode_time->begin().value();
	}
}

json Bomb::SaveToJson(const json& _str)
{
	json data;

	data["Type"] = "Bomb";

	json comp_data;
	comp_data["Explode_Time"] = GetExplodeTime();

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
