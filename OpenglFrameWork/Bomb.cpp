#include "Bomb.h"
#include "GameObject.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "TileEditor.h"
#include "Sprite.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "TimeManager.h"
#include <iostream>

Bomb::Bomb(GameObject* _owner)
	:BaseComponent(_owner)
{	
}

Bomb::~Bomb()
{
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
	if (AccTime >= m_fExplodeTime)
	{
		SetIsExplode(true);
		GameObjectManager::GetInstance()->RemoveObject(m_pOwner->GetID(), m_pOwner->GetName());
		AccTime = 0.f;
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
