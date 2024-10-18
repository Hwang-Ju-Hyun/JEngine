#include "MainEditor.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>
#include "GLHelper.h"
#include "BaseComponent.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "ModelManager.h"
#include "single.h"
#include "Serializer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Sprite.h"
#include "GLHelper.h"
#include "TileEditor.h"

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
            m_bObjectCreateBtn = true;
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
                if (m_bObjectCreateBtn && ImGui::Button("Create GameObject"))
                {                 
                    auto id = GameObjectManager::GetInstance()->GetAllObject().size();                    
                    m_pSelectedGameObject = new GameObject("tempObject", id++);
                    if (m_bCheckBoxTransform)
                    {
                        m_pSelectedGameObject->AddComponent("Transform", new Transform(m_pSelectedGameObject));
                        Transform* trans = static_cast<Transform*>(m_pSelectedGameObject->FindComponent("Transform"));
                        trans->SetPosition({ position.x, position.y });
                        trans->SetScale({ scale.x, scale.y });
                    }
                    if (m_bCheckBoxSprite)
                    {
                        m_pSelectedGameObject->AddComponent("Sprite", new Sprite(m_pSelectedGameObject));
                        Sprite* sprite = static_cast<Sprite*>(m_pSelectedGameObject->FindComponent("Sprite"));
                        sprite->SetColor(color);
                    }

                    GLModel* model = ModelManager::GetInstance()->FindModel(MODEL_TYPE::TRIANGLE);
                    m_pSelectedGameObject->SetModelType(model->GetModelType());
                    
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
        s.reserve(200);

        for (int i = 0; i < 200; i++)
            s.append(" ");

        ImGui::Text(s.c_str());

        if (ImGui::BeginMenu("Save"))
        {
            m_bShowSaveConf = true;
            if (m_bShowSaveConf)
            {
                ImGui::Text("Are you sure to save?");
                if (ImGui::Button("YES"))
                {                    
                    Serializer::GetInstance()->SaveGameObject("json/GameObject/GameObject.json");
                    Serializer::GetInstance()->SaveWall("json/GameObject/Wall.json");
                    m_bShowSaveConf = false;
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("NO"))
                {
                    m_bShowSaveConf = false;
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
    if(ImGui::Begin("Object List"))
        m_bCurWindowObjectList = true;
    else
        m_bCurWindowObjectList = false;

    for (auto obj : all_objs)
    {        
        if (ImGui::Button((obj->GetName()+std::to_string(obj->GetID())).c_str()))
        {                       
            m_pSelectedGameObject = obj;
        }
    }
    
    if (m_pSelectedGameObject)
    {        
        GLModel* model= m_pSelectedGameObject->GetModel();
        GLModel* NewModel = nullptr;
        Transform* transform = static_cast<Transform*>(m_pSelectedGameObject->FindComponent("Transform"));
        Sprite* sprite = static_cast<Sprite*>(m_pSelectedGameObject->FindComponent("Sprite"));
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
            if (NewModel != nullptr)
            {
                m_pSelectedGameObject->SetModelType(NewModel->GetModelType());                
            }
        }
        if (ImGui::Button("DeleteObject"))
        {
            m_bShowDeleteConf = true;
        }
        if (m_bShowDeleteConf)
        {
            ImGui::Begin("Are you sure to delete?");
            if (ImGui::Button("YES"))
            {
                auto obj_id = m_pSelectedGameObject->GetID();
                GameObjectManager::GetInstance()->RemoveObject(obj_id);
                m_pSelectedGameObject = nullptr;
                m_bShowDeleteConf = false;
            }
            else if (ImGui::Button("No"))
            {
                m_bShowDeleteConf = false;
            }
            ImGui::End();
        }
    }    
    ImGui::End();
}

void MainEditor::TopBar_TileEdit()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("ChangeEditMode"))
        {
            m_bShowChangeEditModeConf = true;
            if (m_bShowChangeEditModeConf)
            {                       
                if (ImGui::TreeNode("MODE"))
                {                    
                    if (ImGui::Button("Normal"))
                    {                        
                        ChangeCurrentEditMode(EDIT_MODE::NORMAL);
                        ImGui::CloseCurrentPopup();
                    }
                    else if (ImGui::Button("TileEdit"))
                    {                        
                        ChangeCurrentEditMode(EDIT_MODE::TILE);
                        ImGui::CloseCurrentPopup();                        
                    }
                    m_bShowChangeEditModeConf = false;
                    ImGui::TreePop();
                }                                
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
}


void MainEditor::SelectedObjectByMouse()
{    
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();    
    auto L_mouse_trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();
    auto screen_mouse_pos = GLHelper::GetInstance()->GetMouseCursorPosition();    
    for (const auto& obj : all_objs)
    {       
        Transform* trans = dynamic_cast<Transform*>(obj->FindComponent("Transform"));
        if (trans!=nullptr)
        {                    
            float left   = trans->GetPosition().x - trans->GetScale().x / 2.f;
            float right  = trans->GetPosition().x + trans->GetScale().x / 2.f;
            float top    = trans->GetPosition().y + trans->GetScale().y / 2.f;
            float bottom = trans->GetPosition().y - trans->GetScale().y / 2.f;                        
            glm::vec2 vertex;
                            
            if (!m_bSelectedObjByMouse && GLHelper::GetInstance()->IsPointInsideRectangle(screen_mouse_pos, left,right,top,bottom,false));
            {
                m_pTransByMouseSelect = static_cast<Transform*>(obj->FindComponent("Transform"));
                if (L_mouse_trigger)
                {

                    std::cout << "Pos : " << trans->GetPosition().x << "," << trans->GetPosition().y << std::endl;
                    std::cout << "left : " << left << std::endl;
                    std::cout << "right : " << right << std::endl;
                    std::cout << "top : " << top << std::endl;
                    std::cout << "bottom : " << bottom << std::endl;
                    std::cout << std::endl;

                    if (m_pTransByMouseSelect == nullptr)
                    {
                        std::cerr << "Error : Object Can't find Transform Component - MainEditor:: SelectedObjectByMouse" << std::endl;
                        return;
                    }
                    m_bSelectedObjByMouse = true;
                }
            }
        }                
    }           
    if (m_bSelectedObjByMouse)
    {                               
        m_pTransByMouseSelect->SetPosition({m_vWorldMousePos.x,m_vWorldMousePos.y});
    }    
    if (!L_mouse_trigger)
    {                        
        m_bSelectedObjByMouse = false;        
    }
}

void MainEditor::ChangeCurrentEditMode(enum EDIT_MODE _eMode)
{
    m_iCurrentMode = _eMode;
}

int MainEditor::GetCurrentEditMode() const
{
    return m_iCurrentMode;
}

void MainEditor::Update()
{
    auto ScreenToWorld = GLHelper::GetInstance()->GetScreenToWorldMatFromMouse();
    m_vWorldMousePos = { ScreenToWorld[2][0],ScreenToWorld[2][1] };            
    
    std::cout <<"World Mouse Pos : "<< m_vWorldMousePos.x << "," << m_vWorldMousePos.y << std::endl;

    switch (GetCurrentEditMode())
    {
    case EDIT_MODE::NORMAL:
        if(!m_bCurWindowObjectList)
            SelectedObjectByMouse();
        break;
    case EDIT_MODE::TILE:
        if(!m_bCurWindowObjectList)
            TileEditor::GetInstance()->Update();
        break;
    default:
        break;
    }

    TopBar_GameObject();
    TopBar_TileEdit();
    TopBar_Save();
    SelectedObjectWindow();
}