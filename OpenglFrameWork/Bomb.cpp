#include "Bomb.h"
#include "GameObject.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "TileEditor.h"
#include "Sprite.h"
#include "Player.h"
#include "GameObjectManager.h"
#include <iostream>

Bomb::Bomb(GameObject* _owner)
	:BaseComponent(_owner)
{	
}

Bomb::~Bomb()
{
}

void Bomb::AddExplodeTime(float _time)
{
	m_fExplodeTime += _time;
}

const float Bomb::GetExplodeTime() const
{
	return m_fExplodeTime;
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
