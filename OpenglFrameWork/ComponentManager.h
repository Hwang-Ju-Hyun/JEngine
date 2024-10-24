#pragma once
#include "single.h"
#include <string>
#include <vector>

class BaseComponent;

class ComponentManager
{
public:
	SINGLE(ComponentManager);
public:	
	std::vector<BaseComponent*> m_vecComponent;
public:
	bool Update();
public:
	BaseComponent* AddComponent(const std::string _compName,BaseComponent* _comp);		
	std::vector<BaseComponent*> GetAllComponents() const;
	void RemoveComponent(BaseComponent* _comp);
};

