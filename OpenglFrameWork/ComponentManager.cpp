#include "ComponentManager.h"
#include "BaseComponent.h"
#include "Player.h"
#include "GameObjectManager.h"
#include <iostream>

ComponentManager::ComponentManager()
{

}

ComponentManager::~ComponentManager()
{

}

bool ComponentManager::Update()
{
	auto all_compoents = m_vecComponent;

	for (auto comp : all_compoents)
	{
		if (comp != nullptr)
			comp->Update();
		else
		{
			std::cerr << "Error : Component is nullptr - ComponentManager::Update" << std::endl;
			return false;
		}			
	}
	return true;
}

BaseComponent* ComponentManager::AddComponent(const std::string _compName,BaseComponent* _comp)
{
	if (_comp == nullptr)
	{
		std::cerr << "Error : Component is Invalid - ComponentManager::AddComponent" << std::endl;
		return nullptr;
	}
	_comp->SetName(_compName);
	m_vecComponent.push_back(_comp );

	if (_compName == Player::PlayerTypeName)
	{
		Player* player_comp = static_cast<Player*>(_comp);
		GameObjectManager::GetInstance()->AddPlayerToPlayerVec(player_comp);
	}

	int size = m_vecComponent.size();
	return m_vecComponent[size - 1];
}

std::vector<BaseComponent*> ComponentManager::GetAllComponents() const
{
	return m_vecComponent;
}

void ComponentManager::RemoveComponent(BaseComponent* _comp)
{
	for(auto iter=m_vecComponent.begin();iter!=m_vecComponent.end();iter++)
	{
		if (*iter == _comp)
		{
			m_vecComponent.erase(iter);
			break;
		}			
	}
}
