#include "CollisionManager.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Transform.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

bool CollisionManager::IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2)
{
	Transform* obj_trs1 = (Transform*)_obj1->FindComponent("Transform");
	Transform* obj_trs2 = (Transform*)_obj2->FindComponent("Transform");
	
	glm::vec2 obj1_Pos = static_cast<Transform*>(obj_trs1)->GetPosition();
	glm::vec2 obj2_Pos = static_cast<Transform*>(obj_trs2)->GetPosition();

	glm::vec2 obj1_Scale = static_cast<Transform*>(obj_trs1)->GetScale();
	glm::vec2 obj2_Scale = static_cast<Transform*>(obj_trs2)->GetScale();

	float obj1Right = obj1_Pos.x + obj1_Scale.x / 2.f;
	float obj1Left  = obj1_Pos.x - obj1_Scale.x / 2.f;
	float obj1Top   = obj1_Pos.y + obj1_Scale.y / 2.f;
	float obj1Bot   = obj1_Pos.y - obj1_Scale.y / 2.f;

	float obj2Right = obj2_Pos.x + obj2_Scale.x / 2.f;
	float obj2Left  = obj2_Pos.x - obj2_Scale.x / 2.f;
	float obj2TopY  = obj2_Pos.y - obj2_Scale.y / 2.f;
	float obj2BotY  = obj2_Pos.y + obj2_Scale.y / 2.f;
    return false;
}

bool CollisionManager::IsCollisionRectAndTri(GameObject* _obj1, GameObject* _obj2)
{
    return false;
}
