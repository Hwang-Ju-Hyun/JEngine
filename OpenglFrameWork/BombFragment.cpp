#include "BombFragment.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>

BombFragment::BombFragment(GameObject* _owner)
	:BaseComponent(_owner)
{
}

BombFragment::~BombFragment()
{
}

void BombFragment::SetExplodingTime(float _time)
{
	m_fExplodingTime = _time;
}

float BombFragment::GetExplodingTime() const
{
	return m_fExplodingTime;
}

void BombFragment::LoadFromJson(const json& _str)
{
	auto comp_data = _str.find("CompData");
	if (comp_data != _str.end())
	{		
		auto explode_time = comp_data->find("Exploding_Time");
		m_fExplodingTime = explode_time->begin().value();		
	}
}

json BombFragment::SaveToJson(const json& _str)
{
	json data;
	data["Type"] = "BombFragment";

	json comp_data;	
	comp_data["Exploding_Time"] = GetExplodingTime();

	data["CompData"] = comp_data;
	return data;
}

BaseRTTI* BombFragment::CreateBombFragComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent(BombFragment::BombFragmentTypeName, new BombFragment(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - Bomb::CreateBombComponent" << std::endl;
		return nullptr;
	}
	return p;
}
