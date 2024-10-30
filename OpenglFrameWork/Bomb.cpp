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

void Bomb::SpawnBomb()
{			
	auto grid = TileEditor::GetInstance()->m_sWallGridCoord;	
	Transform* master_trs = static_cast<Transform*>(m_pBombMaster->FindComponent(Transform::TransformTypeName));
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (grid[i][j])
			{
				m_pBomb = new GameObject("Bomb", id);				
				Transform* bomb_trs = (Transform*)m_pBomb->AddComponent(Transform::TransformTypeName, new Transform(m_pBomb));				
				TileEditor::GetInstance()->GetScreenGridByPoint(bomb_trs->GetPosition());
				bomb_trs->SetPosition(master_trs->GetPosition());
				m_pBomb->AddComponent(Sprite::SpriteTypeName, new Sprite(m_pBomb));
				m_pBomb->SetModelType(MODEL_TYPE::CIRCLE);
			}
		}
	}	
		
}