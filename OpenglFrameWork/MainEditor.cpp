#include "MainEditor.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include <iostream>
#include "GLHelper.h"
#include "BaseComponent.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "ModelManager.h"
#include "single.h"
#include "Stage01_Lvl.h"
#include "Stage02_Lvl.h"
#include "Serializer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Sprite.h"
#include "GLHelper.h"
#include "TextureResource.h"
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
                    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();

                    int object_last_id = -1;
                    for (int i = 0; i < all_objs.size(); i++)
                    {
                        if (all_objs[i]->GetName() == "tempObject")
                        {
                            object_last_id = all_objs[i]->GetID() + 1;
                        }
                    }                                 
                    m_pSelectedGameObject = new GameObject("tempObject", object_last_id++);
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
        if (ImGui::TreeNode("Change Level"))
        {
            auto cur_level = GameStateManager::GetInstance()->GetCurrentLevel();
            if (ImGui::Button("Stage01_Lvl"))
            {                
                if (cur_level->GetName() != "Stage01_Lvl")
                    GameStateManager::GetInstance()->ChangeLevel(new Stage01_Lvl("Stage01_Lvl"));
            }
            if (ImGui::Button("Stage02_Lvl"))
            {
                if (cur_level->GetName() != "Stage01_Lv2")
                    GameStateManager::GetInstance()->ChangeLevel(new Stage02_Lvl("Stage02_Lvl"));
            }
            ImGui::TreePop();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void MainEditor::TopBar_Save()
{
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
    
    if (ImGui::BeginMainMenuBar())
    {

        std::string s;
        s.reserve(200);

        for (int i = 0; i < 230; i++)
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
                    for (int i = 0; i < all_objs.size(); i++)
                    {
                        if (all_objs[i]->GetName() == "tempObject")
                        {
                            static int object_id = 1;
                            all_objs[i]->SetID(object_id++);
                        }                        
                    }
                        
                    for (int i = 0; i < all_objs.size(); i++)
                    {
                        if (all_objs[i]->GetName() == "WALL")
                        {
                            static int wall_id = 1;
                            all_objs[i]->SetID(wall_id++);
                        }
                    }
                    auto current_level=GameStateManager::GetInstance()->GetCurrentLevel();
                    std::string cur_level_str=current_level->GetName();
                    std::string object_path = "json/" + cur_level_str + "/GameObject.json";
                    std::string wall_path = "json/" + cur_level_str + "/Wall.json";
                    std::string player_path = "json/" + cur_level_str + "/Player.json";
                    Serializer::GetInstance()->SaveGameObject(object_path);
                    Serializer::GetInstance()->SaveWall(wall_path);
                    Serializer::GetInstance()->SavePlayer(player_path);
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

void MainEditor::TopBar_ChangeEditMode()
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


    //auto a =GLHelper::GetInstance()->GetScreenToWorldMat(screen_mouse_pos);
    //glm::vec2 temp = { a[2][0],a[2][1] };
    //std::cout << temp.x << " , " << temp.y << std::endl;
        
    auto HelperInst = GLHelper::GetInstance();
    for (const auto& obj : all_objs)
    {       
        Transform* trans = dynamic_cast<Transform*>(obj->FindComponent("Transform"));
        if (trans!=nullptr)
        {                                                                   
            if(!m_bSelectedObjByMouse)
            {   
                bool mouse_inside_obj = false;

                switch (obj->GetModelType())
                {
                case MODEL_TYPE::TRIANGLE:
                {
                    glm::vec2 tri_top = { trans->GetPosition().x - (trans->GetScale().x / 2.f), trans->GetPosition().y + (trans->GetScale().y / 2.f) };
                    glm::vec2 tri_bottom_left = { trans->GetPosition().x - (trans->GetScale().x / 2.f) ,trans->GetPosition().y - (trans->GetScale().y / 2.f) };
                    glm::vec2 tri_bottom_right = { trans->GetPosition().x + (trans->GetScale().x / 2.f) ,trans->GetPosition().y - (trans->GetScale().y / 2.f) };                    
                    mouse_inside_obj = HelperInst->IsPointInsideTriangle(screen_mouse_pos, tri_top, tri_bottom_left, tri_bottom_right, false);
                    break;
                }                    
                case MODEL_TYPE::RECTANGLE:
                {
                    float rec_left = trans->GetPosition().x - trans->GetScale().x / 2.f;
                    float rec_right = trans->GetPosition().x + trans->GetScale().x / 2.f;
                    float rec_top = trans->GetPosition().y + trans->GetScale().y / 2.f;
                    float rec_bottom = trans->GetPosition().y - trans->GetScale().y / 2.f;
                    mouse_inside_obj = HelperInst->IsPointInsideRectangle(screen_mouse_pos, rec_left, rec_right, rec_top, rec_bottom, false);
                    break;
                }
                case MODEL_TYPE::CIRCLE:
                {
                    glm::vec2 circle_pos = trans->GetPosition();
                    float radius = trans->GetScale().x;
                    mouse_inside_obj = HelperInst->IsPointInsideCircle(screen_mouse_pos,circle_pos,radius, false);
                }
                default:
                    break;
                }

                if (mouse_inside_obj)
                {
                    m_pTransByMouseSelect = dynamic_cast<Transform*>(obj->FindComponent("Transform"));
                    if (L_mouse_trigger)
                    {
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
    }           
    if (m_bSelectedObjByMouse)
    {              
        if(GetCurrentEditMode()==EDIT_MODE::NORMAL)
            m_pTransByMouseSelect->SetPosition({ m_vWorldMousePos.x,m_vWorldMousePos.y });
        else if (GetCurrentEditMode() == EDIT_MODE::TILE)
        {
            if (HelperInst->GetLeftMouseTriggered() && HelperInst->GetLeftControlPressed())
            {
            auto obj_id = m_pTransByMouseSelect->GetOwner()->GetID();
            GameObjectManager::GetInstance()->RemoveObject(obj_id);
            glm::vec2 grid = TileEditor::GetInstance()->GetScreenGridByMousePos(screen_mouse_pos);
            TileEditor::GetInstance()->SetWallGridCoord(grid.x, grid.y, false);
            HelperInst->ResetLeftMouseTriggered();
            }
        }
        
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
    
    if (!m_bCurWindowObjectList)
        SelectedObjectByMouse();
    if (GetCurrentEditMode() == EDIT_MODE::TILE&& !m_bCurWindowObjectList)
    {        
        TileEditor::GetInstance()->Update();
    }        
    TopBar_GameObject();
    TopBar_ChangeEditMode();
    TopBar_Save();
    SelectedObjectWindow();
}