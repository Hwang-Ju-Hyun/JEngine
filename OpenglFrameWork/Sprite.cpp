#include "Sprite.h"
#include <iostream>
#include "GameObject.h"
#include "GameObjectManager.h"
#include "json.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Sprite::Sprite(GameObject* _owner)
	:BaseComponent(_owner)
{
	SetName("Sprite");
}

Sprite::~Sprite()
{
}


void Sprite::Update()
{
	
}

BaseRTTI* Sprite::CreateSpriteComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent("Sprite", new Sprite(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - Sprite::CreateSpriteComponent" << std::endl;
		return nullptr;
	}
	return p;
}

void Sprite::LoadFromJson(const json& str)
{
	auto comp_data = str.find("CompData");
	if (comp_data != str.end())
	{
		auto red = comp_data->find("RED");
		m_vColor[0] = red->begin().value();

		auto green = comp_data->find("GREEN");
		m_vColor[1] = green->begin().value();

		auto blue = comp_data->find("BLUE");
		m_vColor[2] = blue->begin().value();
		
		auto alpha = comp_data->find("ALPHA");
		m_vColor[3] = alpha->begin().value();
	}
}

json Sprite::SaveToJson(const json& str)
{
	json data;
	data["Type"] = "Sprite";

	json compData;
	compData["RED"] =   m_vColor[0];
	compData["GREEN"] = m_vColor[1];
	compData["BLUE"] =  m_vColor[2];
	compData["ALPHA"] = m_vColor[3];

	data["CompData"] = compData;
	return data;
}

bool Sprite::EditFromImgui()
{
	if (BaseComponent::EditFromImgui())
	{
		ImGui::InputFloat4("Color", &m_vColor[0]);
		ImGui::SliderFloat4("Color", &m_vColor[0], 0.0f, 1.f);//요거이 질문
		ImGui::ColorEdit3("Color", (float*)&m_vColor);
	}
	return true;
}
