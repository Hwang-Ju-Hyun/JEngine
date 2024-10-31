#include "Bomb.h"
#include "GameObject.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "TileEditor.h"
#include "Sprite.h"
#include "Player.h"

int Bomb::id = 0;

Bomb::Bomb(GameObject* _owner)
	:BaseComponent(_owner)
{	
}

Bomb::~Bomb()
{
}

void Bomb::SetBombMaster(GameObject* _master)
{
	m_pBombMaster = _master;
}

GameObject* Bomb::GetBombMaster() const
{
	return m_pBombMaster;
}

void Bomb::SpawnBomb(GameObject* _bomb)
{				
	auto grid = TileEditor::GetInstance()->m_sWallGridCoord;	
	Transform* master_trs = static_cast<Transform*>(m_pBombMaster->FindComponent(Transform::TransformTypeName));
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (grid[i][j])
			{				
				Transform* bomb_trs = (Transform*)_bomb->AddComponent(Transform::TransformTypeName, new Transform(_bomb));				
				TileEditor::GetInstance()->GetScreenGridByPoint(bomb_trs->GetPosition());
				bomb_trs->SetPosition(master_trs->GetPosition());
				_bomb->AddComponent(Sprite::SpriteTypeName, new Sprite(_bomb));
				_bomb->SetModelType(MODEL_TYPE::CIRCLE);
			}
		}
	}	
		
}

void Bomb::LoadFromJson(const json& _str)
{
}

json Bomb::SaveToJson(const json& _str)
{
	return json();
}
