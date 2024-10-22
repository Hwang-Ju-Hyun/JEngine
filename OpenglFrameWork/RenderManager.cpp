#include "RenderManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "GameObject.h"
#include "GLShader.h"
#include "Transform.h"
#include "GLModel.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "Sprite.h"
#include <iostream>
#include "GLApp.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

RenderManager::RenderManager()
{

}

RenderManager::~RenderManager()
{

}

bool RenderManager::Init()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

bool RenderManager::Update()
{    
    return true;
}

#include <../GLM/gtc/type_ptr.hpp>

bool RenderManager::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	auto shaders = ShaderManager::GetInstance()->GetAllShader();	
	auto objects = GameObjectManager::GetInstance()->GetAllObject();
	auto models = ModelManager::GetInstance()->GetAllModel();
	auto comps = ComponentManager::GetInstance()->GetAllComponents();
	
	for (auto obj : objects)
	{				
		auto shader_ref = obj->GetShaderRef();				
				
		shaders[shader_ref].Use();				
		
		auto obj_comps=obj->GetAllComponentOfObj();

		for (auto comp : obj_comps)
		{
			if (comp.first == "Transform")
			{
				Transform* trs = static_cast<Transform*>(comp.second);
				//유니폼값의 주소를 리턴
				GLint Model_To_NDC_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uModelToNDC");
				if (Model_To_NDC_Location <= -1)
					std::cerr << "Failed to get uModelToNDC uniform location" << std::endl;
				else
					glUniformMatrix3fv(Model_To_NDC_Location, 1/*전달할 유니폼 갯수*/, GL_FALSE/*전치? 여부*/, glm::value_ptr(trs->GetModelToNDC())/*실제로 전달할 변수의 포인터(적용될 행렬)*/);				
			}
			if (comp.first == "Sprite")
			{
				Sprite* spr = static_cast<Sprite*>(comp.second);
				if (spr->GetTexture())
				{
					GLint Color_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "ourTexture");
					float red(spr->GetColor()[0]), blue(spr->GetColor()[1]), green(spr->GetColor()[2]), alpha(spr->GetColor()[3]);
					if (Color_Location <= -1)
						std::cerr << "Failed to get uOurColor uniform location" << std::endl;
					else
						glUniform4f(Color_Location, red, blue, green, alpha);
				}
				else
				{
					GLint Color_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uOutColor");
					float red(spr->GetColor()[0]), blue(spr->GetColor()[1]), green(spr->GetColor()[2]), alpha(spr->GetColor()[3]);
					if (Color_Location <= -1)
						std::cerr << "Failed to get uOurColor uniform location" << std::endl;
					else
						glUniform4f(Color_Location, red, blue, green, alpha);
				}
			}
		}
		if (obj->GetModel())
			obj->Draw();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return true;
}
