#include "BaseComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Collision.h"
#include "GameObject.h"
#include "Wall.h"
#include <iostream>

BaseComponent::BaseComponent(GameObject* _owner)
	:m_pOwner(_owner)
{
}

BaseComponent::~BaseComponent()
{
}

void BaseComponent::SetName(const std::string _name)
{
	m_strName = _name;
}

const std::string& BaseComponent::GetName() const
{
	return m_strName;
}

void BaseComponent::Update()
{
}

bool BaseComponent::EditFromImgui()
{	
	if (ImGui::TreeNode(GetName().c_str()))
	{		
		ImGui::TreePop();
		return true;
	}
	return false;
}

GameObject* BaseComponent::GetOwner() const
{
	if (m_pOwner == nullptr)
	{
		std::cerr << "Error : Owner is nullptr - BaseComponent::GetOwner" << std::endl;
		return nullptr;
	}
	return m_pOwner;
}

void BaseComponent::EventCollision(Collision* _pOther)
{

}