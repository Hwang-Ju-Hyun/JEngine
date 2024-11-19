#include "BombFragment.h"

BombFragment::BombFragment(GameObject* _owner)
	:BaseComponent(_owner)
{
}

BombFragment::~BombFragment()
{
}

void BombFragment::LoadFromJson(const json& _str)
{
	auto comp_data = _str.find("CompData");
	if (comp_data != _str.end())
	{
		auto remain_time = comp_data->find("Remain_Time");
		m_fRemaingTime = remain_time->begin().value();
		auto explode_time = comp_data->find("Exploding_Time");
		m_fExplodingTime = explode_time->begin().value();
		auto is_fragment = comp_data->find("IsFragment");
		m_bIsFragmenet = is_fragment->begin().value();
	}
}

json BombFragment::SaveToJson(const json& _str)
{
	return json();
}

BaseRTTI* BombFragment::CreateBombComponent()
{
	return nullptr;
}
