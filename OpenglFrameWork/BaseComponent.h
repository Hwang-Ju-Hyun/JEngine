#pragma once
#include <string>
#include "BaseRTTI.h"
class GameObject;

class BaseComponent:public BaseRTTI
{
public:
	BaseComponent()=delete;
	BaseComponent(GameObject* _owner);
	virtual ~BaseComponent();
private:
	std::string m_strName = "Component Nothing";
public:
	void SetName(const std::string _name);
	const std::string& GetName()const;
public:
	virtual void Update();
	virtual bool EditFromImgui();
public:
	GameObject* m_pOwner;
};