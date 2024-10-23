#include "Sprite.h"
#include <iostream>
#include "GameObject.h"
#include "GameObjectManager.h"
#include "TextureResource.h"
#include "json.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "ResourceManager.h"
#include "Resource.h"


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

void Sprite::SetTexture(TextureResource* _texture)
{
	m_pTexture = _texture;
}

TextureResource* Sprite::GetTexture() const
{
	return m_pTexture;
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

		auto texture_name = comp_data->find("TextureName");
		auto texture_path = comp_data->find("TexturePath");

		if (texture_name != comp_data->end() && texture_path != comp_data->end())
		{
			auto resource = ResourceManager::GetInstance()->GetAndLoad(texture_name.value(), texture_path.value());
			resource = static_cast<TextureResource*>(resource);
			SetTexture((TextureResource*)resource);
		}
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
	compData["TextureName"] = m_pTexture->GetName();
	compData["TexturePath"]=m_pTexture->GetPath();

	data["CompData"] = compData;
	return data;
}

TextureResource* Sprite::GetTextureFromImGui() 
{	
	auto resources = ResourceManager::GetInstance()->GetAllResources();	
	std::string str=m_pOwner->GetName() + std::to_string(m_pOwner->GetID());	
	if (ImGui::Begin(str.c_str()),m_bCurTextureWindowConf)
	{				
		if (ImGui::TreeNode("Texture"))
		{
			for (auto iter = resources.begin(); iter != resources.end(); iter++)
			{
				if (ImGui::Button(iter->second->GetName().c_str()))
				{														
					ImGui::TreePop();
					ImGui::End();
					return (TextureResource*)iter->second;				
				}				
			}
			if (ImGui::Button("No Texture"))
			{								
				ImGui::TreePop();
				ImGui::End();
				return nullptr;
			}	
			ImGui::TreePop();
		}		
	}
	else
	{		
		m_bCurTextureWindowConf = false;		
		ImGui::End();
	}	
	ImGui::End();
	return m_pTexture;
}

bool Sprite::EditFromImgui()
{	
	if (BaseComponent::EditFromImgui())
	{
		ImGui::InputFloat4("Color", &m_vColor[0]);		
		ImGui::ColorEdit3("Color", (float*)&m_vColor);	
		m_pTexture=GetTextureFromImGui();		
	}
	return true;
}
