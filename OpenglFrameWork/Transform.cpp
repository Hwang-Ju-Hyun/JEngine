#include "Transform.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <iostream>
#include "json.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


Transform::Transform(GameObject* _owner)
	:BaseComponent(_owner)
{
	SetName("Transform");
}

Transform::~Transform()
{
}

void Transform::SetPosition(glm::vec2 _pos)
{
	m_vPosition = _pos;
}

void Transform::AddPositionX(float _x)
{
	m_vPosition.x += _x;
}

void Transform::AddPositionY(float _y)
{
	m_vPosition.y += _y;
}

void Transform::AddPosition(glm::vec2 _pos)
{
	m_vPosition += _pos;
}

void Transform::SetScale(glm::vec2 _scale)
{
	m_vScale = _scale;
}

void Transform::AddScale(glm::vec2 _scale)
{
	m_vScale += _scale;
}

glm::vec2 Transform::GetPositoin() const
{
	return m_vPosition;
}

glm::vec2 Transform::GetScale() const
{
	return m_vScale;
}

glm::mat3 Transform::GetModelToNDC() 
{
	return m_mModelToNDC;
}

void Transform::Update()
{
	glm::mat3 Transform =
	{
		1,0,0,
		0,1,0,
		m_vPosition.x,m_vPosition.y,1
	};
	glm::mat3 Scale =
	{
		m_vScale.x,0,0,
		0,m_vScale.y,0,
		0,0,1
	};

	glm::mat3 Rot =
	{
		1,0,0,
		0,1,0,
		0,0,1
	};

	glm::mat3 H =
	{
		1.f / 5000.f,0,0,
		0,1.f / 5000.f,0,
		0,0,1
	};
	m_mModelToNDC = Transform * Rot * Scale;
}

BaseRTTI* Transform::CreateTransformComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent("Transform", new Transform(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - Transform::CreateTransformComponent" << std::endl;
		return nullptr;
	}		
	return p;
}

void Transform::LoadFromJson(const json& str)
{
	auto comp_data = str.find("CompData");
	if (comp_data != str.end())
	{
		auto position = comp_data->find("Pos");
		m_vPosition.x = position->begin().value();
		m_vPosition.y = (position->begin()+1).value();

		auto scale = comp_data->find("sca");
		m_vScale.x =  scale->begin().value();
		m_vScale.y = (scale->begin() + 1).value();

		auto rotate = comp_data->find("Rot");
		m_vRotate =  rotate->begin().value();
	}
}

json Transform::SaveToJson(const json& str)
{
	json data;
	//Save the type
	data["Type"] = "Transform";

	//Save my data	
	json compData;
	//pos
	compData["Pos"] = { m_vPosition.x,m_vPosition.y };
	//sca
	compData["sca"] = { m_vScale.x,m_vScale.y };
	//rot
	compData["Rot"] = { 0.f,0.f };

	data["CompData"] = compData;

	return data;
}

bool Transform::Edit()
{
	if (BaseComponent::Edit())
	{
		ImGui::InputFloat2("Pos", &(m_vPosition.x));

		ImGui::TreePop();
	}
	return true;
}
