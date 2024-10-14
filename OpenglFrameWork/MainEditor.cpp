#include "MainEditor.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>
#include "BaseComponent.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "ModelManager.h"
#include "header.h"
#include "Serializer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Sprite.h"
#include "GLHelper.h"


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

        static glm::vec4 color(0.5f);

        if (ImGui::Button("CreateObject"))
        {
            m_bShowObjectWindow = true;
            m_bCheckBoxTransform = true;
            m_bCheckBoxSprite = true;
            m_bBtnObjectCreate = true;
        }

        if (m_bShowObjectWindow)
        {
            ImGui::Begin("New GameObject", &m_bShowObjectWindow);

            if (ImGui::TreeNode("Components"))
            {
                ImGui::Checkbox("Transform", &m_bCheckBoxTransform);
                ImGui::Checkbox("Sprite", &m_bCheckBoxSprite);
                if (m_bCheckBoxTransform)
                {
                    ImGui::Text("Position");
                    ImGui::InputFloat3("Position", &position[0]);

                    ImGui::Text("Scale");
                    ImGui::InputFloat3("Scale", &scale[0]);
                }                
                if (m_bCheckBoxSprite)
                {
                    ImGui::Text("Color");
                    ImGui::InputFloat4("Color", &color[0]);                    
                }
                if (m_bBtnObjectCreate && ImGui::Button("Create GameObject"))
                {                     
                    m_ptrSelectedGameObject = new GameObject("tempObject", tempObjectID++);
                    if (m_bCheckBoxTransform)
                    {
                        m_ptrSelectedGameObject->AddComponent("Transform", new Transform(m_ptrSelectedGameObject));
                        Transform* trans = static_cast<Transform*>(m_ptrSelectedGameObject->FindComponent("Transform"));
                        trans->SetPosition({ position.x, position.y });
                        trans->SetScale({ scale.x, scale.y });
                    }
                    if (m_bCheckBoxSprite)
                    {
                        m_ptrSelectedGameObject->AddComponent("Sprite", new Sprite(m_ptrSelectedGameObject));
                        Sprite* sprite = static_cast<Sprite*>(m_ptrSelectedGameObject->FindComponent("Sprite"));
                        sprite->SetColor(color);
                    }

                    GLModel* model = ModelManager::GetInstance()->FindModel(MODEL_TYPE::TRIANGLE);
                    m_ptrSelectedGameObject->SetModelType(model->GetModelType());
                    
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

        std::string s;
        s.reserve(250);

        for (int i = 0; i < 250; i++)
            s.append(" ");

        ImGui::Text(s.c_str());

        if (ImGui::BeginMenu("Save"))
        {
            m_bShowSaveConfirmation = true;
            if (m_bShowSaveConfirmation)
            {
                ImGui::Text("Are you sure to save?");
                if (ImGui::Button("YES"))
                {                    
                    Serializer::GetInstance()->SaveGameObject("json/GameObject/GameObject.json");
                    m_bShowSaveConfirmation = false;
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("NO"))
                {
                    m_bShowSaveConfirmation = false;
                    ImGui::CloseCurrentPopup();
                }
            }         
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MainEditor::SelectedObjectWindow()
{
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();    
    ImGui::Begin("Object List");        
    for (auto obj : all_objs)
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
        Sprite* sprite = static_cast<Sprite*>(m_ptrSelectedGameObject->FindComponent("Sprite"));
        if (transform)
        {     
            transform->EditFromImgui();
        }
        if (sprite)
        {
            sprite->EditFromImgui();
        }
        if (model)
        {
            NewModel = model->GetModelFromImgui();
            if (NewModel!=nullptr)
            {                
                m_ptrSelectedGameObject->SetModelType(NewModel->GetModelType());
            }            
        }
        if (ImGui::Button("DeleteObject"))
        {
            m_bShowDeleteConfirmationWindow = true;           
        }
        if (m_bShowDeleteConfirmationWindow)
        {
            ImGui::Begin("Are you sure to delete?");
            if (ImGui::Button("YES"))
            {
                auto obj_id = m_ptrSelectedGameObject->GetID();
                GameObjectManager::GetInstance()->RemoveObject(obj_id);
                m_ptrSelectedGameObject = nullptr;                
                m_bShowDeleteConfirmationWindow = false;
            }
            else if (ImGui::Button("No"))
            {   
                m_bShowDeleteConfirmationWindow = false;
            }
            ImGui::End();
        }
    }    

    ImGui::End();
}

bool MainEditor::IsMouseInsideObject(glm::vec2 _mousePos, GameObject* _obj)
{
    Transform* trs = dynamic_cast<Transform*>(_obj->FindComponent("Transform"));
    if (trs == nullptr)    
        return false;    

    float Left = trs->GetPosition().x - trs->GetScale().x / 2.f;
    float Right = trs->GetPosition().x + trs->GetScale().x / 2.f;
    float Top = trs->GetPosition().y - trs->GetScale().y / 2.f;
    float Bottom = trs->GetPosition().y + trs->GetScale().y / 2.f;

    if (_mousePos.x >= Left && _mousePos.x <= Right
        && _mousePos.y >= Top && _mousePos.y <= Bottom)    
        return true;    
        
   std::cout << "L : " << Left << std::endl;
   std::cout << "R : " << Right << std::endl;
   std::cout << "T : " << Top << std::endl;
   std::cout << "B : " << Bottom << std::endl;
   std::cout<<std::endl;

   auto a= trs->GetWorldToScreen();
   a;
}

void MainEditor::SelectedObjectByMouse()
{
    glm::vec2 mouse_cur_pos = GLHelper::GetInstance()->GetMouseCursorPosition();
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
    auto left_mouse_trigger=GLHelper::GetInstance()->GetLeftMouseTriggered();
    for (const auto& obj : all_objs)
    {
        if (IsMouseInsideObject(mouse_cur_pos,obj)&&obj->GetID()==4)
        {   
            if (left_mouse_trigger)
            {
                int a = 0;
                left_mouse_trigger = false;
            }
            
        }
    }
}



void MainEditor::Update()
{
    TopBar_GameObject();
    SelectedObjectByMouse();
    SelectedObjectWindow();
    TopBar_Save();
}