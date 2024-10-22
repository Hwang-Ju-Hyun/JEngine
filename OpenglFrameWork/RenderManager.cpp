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

    for (auto obj : objects)
    {                
        auto shader_ref = obj->GetShaderRef();                
        shaders[shader_ref].Use();                
        
        auto obj_comps = obj->GetAllComponentOfObj();

        for (auto comp : obj_comps)
        {
            if (comp.first == "Transform")
            {
                Transform* trs = static_cast<Transform*>(comp.second);
                GLint Model_To_NDC_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uModelToNDC");
                if (Model_To_NDC_Location <= -1)
                    std::cerr << "Failed to get uModelToNDC uniform location" << std::endl;
                else
                    glUniformMatrix3fv(Model_To_NDC_Location, 1, GL_FALSE, glm::value_ptr(trs->GetModelToNDC()));
            }

            if (comp.first == "Sprite")
            {
                Sprite* spr = static_cast<Sprite*>(comp.second);
                if (spr->GetTexture()) // 텍스처가 있는 경우
                {
                    // 텍스처 유니폼 처리
                    GLint Texture_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "ourTexture");
                    GLint Has_Texture_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "hasTexture");
                    
                    if (Texture_Location <= -1 || Has_Texture_Location <= -1)
                    {
                        std::cerr << "Failed to get uniform location" << std::endl;
                    }
                    else
                    {
                        glUniform1i(Texture_Location, 0); // 텍스처 슬롯 0 사용
                        glUniform1i(Has_Texture_Location, true); // 텍스처가 있음
                    }
                }
                else // 텍스처가 없는 경우
                {
                    // RGB 색상 유니폼 처리
                    GLint Color_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uOutColor");
                    GLint Has_Texture_Location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "hasTexture");

                    if (Color_Location <= -1 || Has_Texture_Location <= -1)
                    {
                        std::cerr << "Failed to get uniform location" << std::endl;
                    }
                    else
                    {
                        float red = spr->GetColor()[0];
                        float green = spr->GetColor()[1];
                        float blue = spr->GetColor()[2];
                        float alpha = spr->GetColor()[3];
                        glUniform4f(Color_Location, red, green, blue, alpha); // 색상 유니폼 설정
                        glUniform1i(Has_Texture_Location, false); // 텍스처가 없음
                    }
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