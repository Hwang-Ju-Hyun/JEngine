#include "MainEditor.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>
#include "BaseComponent.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "ModelManager.h"
#include "Serializer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//에디터에서 new level을 구현할 시 selected 꼭 nullptr로 만들자
MainEditor::MainEditor()
{

}


MainEditor::~MainEditor()
{

}


void MainEditor::TopBar_GameObject()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("GameObject"))
    {                        
        static glm::vec3 position(0.5f);
        static glm::vec3 scale(0.5f);

        if (ImGui::Button("CreateObject"))
        {
            m_bShowObjectWindow = true;
            m_bCheckBoxTransform = true;
            m_bBtnObjectCreate = true; // Reset objectCreated state when opening the window
        }

        if (m_bShowObjectWindow)
        {
            ImGui::Begin("New GameObject", &m_bShowObjectWindow);

            if (ImGui::TreeNode("Components"))
            {
                ImGui::Checkbox("Transform", &m_bCheckBoxTransform);

                if (m_bCheckBoxTransform)
                {
                    ImGui::Text("Position");
                    ImGui::InputFloat3("Position", &position[0]);

                    ImGui::Text("Scale");
                    ImGui::InputFloat3("Scale", &scale[0]);
                }
                
                if (m_bBtnObjectCreate && ImGui::Button("Create GameObject"))
                {                     
                    m_ptrSelectedGameObject = new GameObject("EditObject", tempObjectID++);

                    if (m_bCheckBoxTransform)
                    {
                        m_ptrSelectedGameObject->AddComponent("Transform", new Transform(m_ptrSelectedGameObject));
                        Transform* trans = static_cast<Transform*>(m_ptrSelectedGameObject->FindComponent("Transform"));
                        trans->SetPosition({ position.x, position.y });
                        trans->SetScale({ scale.x, scale.y });
                    }
                    GLModel* model = ModelManager::GetInstance()->FindModel("Triangle");
                    m_ptrSelectedGameObject->SetModel(model);
                    
                    std::cout << "GameObject created!" << std::endl;                    
                }
                ImGui::TreePop();
            }

            ImGui::End();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Load Level"))
    {
        ImGui::MenuItem("Load Level");
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}


void MainEditor::TopBar_Save()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Save"))
        {            
            m_bShowSaveConfirmation = true;
            if (m_bShowSaveConfirmation)
            {
                ImGui::Text("Are you sure to save?");
                if (ImGui::Button("YES"))
                {
                    auto all_objects = GameObjectManager::GetInstance()->GetAllObject();
                    Serializer::GetInstance()->SaveGameObject("json/GameObject/GameObject.json");
                    m_bShowSaveConfirmation = false; 
                }
                if (ImGui::Button("NO"))
                {
                    m_bShowSaveConfirmation = false;
                }
            }         
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MainEditor::SelectedObjectWindow()
{
    ImGui::Begin("Object List");

    for (auto obj : GameObjectManager::GetInstance()->GetAllObject())
    {        
        if (ImGui::Button((obj->GetName()+std::to_string(obj->GetID())).c_str()))
        {                       
            m_ptrSelectedGameObject = obj;
        }
    }
    
    if (m_ptrSelectedGameObject)
    {        
        GLModel* model= m_ptrSelectedGameObject->GetModel();
        GLModel* NewModel = nullptr;
        Transform* transform = static_cast<Transform*>(m_ptrSelectedGameObject->FindComponent("Transform"));
        if (transform)
        {     
            transform->EditFromImgui();
        }
        if (model)
        {
            NewModel = model->EditFromImgui();
            if (NewModel!=nullptr)
            {
                m_ptrSelectedGameObject->SetModel(NewModel);
            }            
        }
    }    

    ImGui::End();
}

void MainEditor::Update()
{
    TopBar_GameObject();
    SelectedObjectWindow();
    TopBar_Save();
}