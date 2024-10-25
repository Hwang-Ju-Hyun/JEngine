#include "CollisionManager.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Player.h"
#include "Transform.h"
#include "Player.h"
#include "Serializer.h"
#include <iostream>

CollisionManager::CollisionManager()
{
	
}

CollisionManager::~CollisionManager()
{

}

bool CollisionManager::IsOverLapRectAndRect(float _left1, float _right1, float _top1, float _bot1, float _left2, float _right2, float _top2, float _bot2)
{
	if (_right1 > _left2 && _left1 < _right2 && _bot1<_top2 && _top1>_bot2)	
		return true;
	return false;
}

bool CollisionManager::IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2)
{	

	Transform* obj_trs1 = (Transform*)_obj1->FindComponent("Transform");
	Transform* obj_trs2 = (Transform*)_obj2->FindComponent("Transform");	
	
	glm::vec2 obj1_Pos = static_cast<Transform*>(obj_trs1)->GetPosition();
	glm::vec2 obj2_Pos = static_cast<Transform*>(obj_trs2)->GetPosition();	

	glm::vec2 obj1_Scale = static_cast<Transform*>(obj_trs1)->GetScale();
	glm::vec2 obj2_Scale = static_cast<Transform*>(obj_trs2)->GetScale();	

	float obj1Left  = obj1_Pos.x - obj1_Scale.x / 2.f;
	float obj1Right = obj1_Pos.x + obj1_Scale.x / 2.f;
	float obj1Top   = obj1_Pos.y + obj1_Scale.y / 2.f;
	float obj1Bot   = obj1_Pos.y - obj1_Scale.y / 2.f;

	float obj2Left  = obj2_Pos.x - obj2_Scale.x / 2.f;
	float obj2Right = obj2_Pos.x + obj2_Scale.x / 2.f;
	float obj2TopY  = obj2_Pos.y + obj2_Scale.y / 2.f;
	float obj2BotY  = obj2_Pos.y - obj2_Scale.y / 2.f;

	if (IsOverLapRectAndRect(obj1Left, obj1Right, obj1Top, obj1Bot, obj2Left, obj2Right, obj2TopY, obj2BotY))
		return true;
    return false;
}

bool CollisionManager::IsRectangleInsideTriangle(GameObject* _obj1, GameObject* _obj2)
{
	
}

bool CollisionManager::IsCollisionRectAndTri(GameObject* _obj1, GameObject* _obj2)
{
    return false;
}


bool CollisionManager::Init()
{	
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	for (auto obj : all_objs)
	{
		auto player_comp = obj->FindComponent(Player::PlayerTypeName);
		if (player_comp != nullptr)
			m_pPlayer = player_comp->GetOwner();
	}
	return true;
}

bool CollisionManager::Update()
{		
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();	
	for (auto obj : all_objs)
	{
		if (obj->GetName() == "WALL"&&obj->GetID()==47)
		{
			if (IsCollisionRectAndRect(m_pPlayer, obj))
			{
				std::cout << "Collision" << std::endl;
			}
			else
			{
				std::cout << "Not Collision" << std::endl;
			}
		}		
	}
	return true;
}
