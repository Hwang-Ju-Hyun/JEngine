#include "Transform.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <iostream>
#include "json.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ShaderManager.h"
#include "GLShader.h"
#include "header.h"
#include "GLModel.h"
#include "TileEditor.h"
#include "GLHelper.h"
#include <../GLM/gtc/type_ptr.hpp>
#include "RigidBody.h"
#include "TimeManager.h"

Transform::Transform(GameObject* _owner)
	:BaseComponent(_owner)
{
	SetName("Transform");
}

Transform::~Transform()
{
}

void Transform::SetGridByScreenPos(glm::vec2 _pos)
{	
	m_vGrid = _pos;
}

glm::vec2 Transform::GetGridByScreenPos() const
{
	return m_vGrid;
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

glm::vec2 Transform::GetPosition() const
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

glm::mat3 Transform::GetScreenByWorld()
{	
	glm::mat3 trans =
	{
		1,0,0,
		0,1,0,
		window_width/2.f,window_height/2.f,1.f
	};
	glm::mat3 X_axis_Revert =
	{
	    1,  0,  0,
		0, -1,  0,
		0,  0,  1
	};	

	m_mWorldToScreen = trans*X_axis_Revert *(m_mModelToWorld);
	m_mWorldToScreen *= X_axis_Revert;
	return m_mWorldToScreen;
}


void Transform::Update()
{
	double dt = TimeManager::GetInstance()->GetDeltaTime();
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
		1.f /( window_width  /2.f),0,0,
		0,1.f/( window_height/2.f),0,
		0,0,1
	};

	m_mModelToWorld = (Transform * Rot * Scale);
	m_mModelToNDC = H * m_mModelToWorld;
	
	auto screen_mat=GetScreenByWorld();	

	int wall_width=TileEditor::GetInstance()->GetWallWidth();
	int wall_height = TileEditor::GetInstance()->GetWallHeight();

	m_vGrid.x = int(screen_mat[2][0] / wall_width);
	m_vGrid.y = int(screen_mat[2][1] / wall_height);
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

		auto grid = comp_data->find("Grid");
		m_vGrid.x = grid->begin().value();
		m_vGrid.y = (grid->begin() + 1).value();
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

	compData["Grid"] = { m_vGrid.x,m_vGrid.y };

	data["CompData"] = compData;

	return data;
}

bool Transform::EditFromImgui()
{
	if (BaseComponent::EditFromImgui())
	{
		ImGui::InputFloat2("Pos", &(m_vPosition[0]));		
		ImGui::InputFloat2("Scale", &(m_vScale[0]));
	}
	return true;
}