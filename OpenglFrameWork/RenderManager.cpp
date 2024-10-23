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
                    GLint texture_location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uOutTexture");
                    GLint has_texture_location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uHasTexture");
                    
                    if (texture_location <= -1 || has_texture_location <= -1)
                    {
                        std::cerr << "Failed to get uniform location" << std::endl;
                    }
                    else
                    {
                        glUniform1i(texture_location, 0); 
                        glUniform1i(has_texture_location, true); 
                    }
                }
                else 
                {
                    
                    GLint color_location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uOutColor");
                    GLint has_texture_location = glGetUniformLocation(shaders[shader_ref].GetShaderPgmHandle(), "uHasTexture");

                    if (color_location <= -1 || has_texture_location <= -1)
                    {
                        std::cerr << "Failed to get uniform location" << std::endl;
                    }
                    else
                    {
                        float red = spr->GetColor()[0];
                        float green = spr->GetColor()[1];
                        float blue = spr->GetColor()[2];
                        float alpha = spr->GetColor()[3];
                        glUniform4f(color_location, red, green, blue, alpha); 
                        glUniform1i(has_texture_location, false); 
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