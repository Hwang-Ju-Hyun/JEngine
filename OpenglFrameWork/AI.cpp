#include "AI.h"
#include "EnemyStateManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "IdleState.h"
#include "Enemy.h"
#include <iostream>

AI::AI(GameObject* _owner)
	:BaseComponent(_owner)
{
	EnemyStateManager::GetInstance()->Init();
}

AI::~AI()
{
}

void AI::Update()
{
	EnemyStateManager::GetInstance()->Update();
}

void AI::SetState(const std::string& _name)
{
	if (_name == Enemy::EnemyTypeName)
	{
		IdleState* idle_state = new IdleState;
		EnemyStateManager::GetInstance()->ChangeState(idle_state);
	}
}

void AI::ChangeState(BaseEnemyState* _state)
{
	EnemyStateManager::GetInstance()->ChangeState(_state);
}

BaseRTTI* AI::CreateAIComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent(AI::AITypeName, new AI(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - AI::CreateAIComponent" << std::endl;
		return nullptr;
	}
	return p;
}

void AI::LoadFromJson(const json& _str)
{
	auto comp_data = _str.find("CompData");
	if (comp_data != _str.end())
	{		
		auto cur_state = comp_data->find("Start State");
		m_sCurState = cur_state->begin().value();
	}
}

json AI::SaveToJson(const json& _str)
{
	json data;
	data["Type"] = "AI";
	json comp_data;
	comp_data["Start State"] = m_sCurState;
	return data;
}