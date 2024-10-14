#include "BaseComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GameObject.h"

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
