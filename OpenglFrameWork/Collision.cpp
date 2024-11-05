#include "Collision.h"
#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Wall.h"
#include <iostream>

Collision::Collision(GameObject* _owner)
	:BaseComponent(_owner)
{
}

Collision::~Collision()
{
}

void Collision::Update()
{
	
}

void Collision::LoadFromJson(const json& _str)
{
}

json Collision::SaveToJson(const json& _str)
{
	return json();
}

BaseRTTI* Collision::CreateCollisionComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent(Collision::CollisionTypeName, new Collision(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - Collision::CreateCollisionComponent" << std::endl;
		return nullptr;
	}
	return p;
}
