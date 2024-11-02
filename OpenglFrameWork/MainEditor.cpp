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
#include "Wall.h"
#include "TextureResource.h"
#include "TileEditor.h"

//에디터에서 new level을 구현할 시 selected 꼭 nullptr로 만들자
MainEditor::MainEditor()
{

}


MainEditor::~MainEditor()
{

}


void MainEditor::TopBar_ShowGrid()
{
    if (ImGui::BeginMenu("Show Grid"))
    {
        ImGui::Checkbox("Grid", &m_bShowGrid);
        GameObject* grid_obj= new GameObject("GridObject",0);
        grid_obj->AddComponent(Transform::TransformTypeName, new Transform(grid_obj));
        grid_obj->AddComponent(Sprite::SpriteTypeName, new Sprite(grid_obj));
        grid_obj->SetModelType(MODEL_TYPE::GRID_LINE);        
        Transform* grid_trs = static_cast<Transform*>(grid_obj->FindComponent(Transform::TransformTypeName));
        grid_trs->SetPosition({ 0.f,0.f });
        grid_trs->SetScale({window_width/2.f,window_height/2.f});
        if (!m_bShowGrid)
        {
            GameObjectManager::GetInstance()->RemoveObjectsByName(grid_obj->GetName());
        }
        ImGui::EndMenu();
    }
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
                    m_pNewObject = new GameObject("tempObject", object_last_id++);
                    if (m_bCheckBoxTransform)
                    {
                        m_pNewObject->AddComponent("Transform", new Transform(m_pNewObject));
                        Transform* trans = static_cast<Transform*>(m_pNewObject->FindComponent("Transform"));
                        trans->SetPosition({ position.x, position.y });
                        trans->SetScale({ scale.x, scale.y });
                    }
                    if (m_bCheckBoxSprite)
                    {
                        m_pNewObject->AddComponent("Sprite", new Sprite(m_pNewObject));
                        Sprite* sprite = static_cast<Sprite*>(m_pNewObject->FindComponent("Sprite"));
                        sprite->SetColor(color);
                    }

                    GLModel* model = ModelManager::GetInstance()->FindModel(MODEL_TYPE::TRIANGLE);
                    m_pNewObject->SetModelType(model->GetModelType());
                    
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
                {                    
                    GameStateManager::GetInstance()->ChangeLevel(new Stage01_Lvl("Stage01_Lvl"));
                }
                    
            }  
            if (ImGui::Button("Stage02_Lvl"))
            {  
                if (cur_level->GetName() != "Stage01_Lv2")
                {
                    TileEditor::GetInstance()->Init();
                    GameStateManager::GetInstance()->ChangeLevel(new Stage02_Lvl("Stage02_Lvl"));
                }                    
            }  
            ImGui::TreePop();
        }
        ImGui::EndMenu();
    }
    TopBar_ShowGrid();
    ImGui::EndMainMenuBar();
}

void MainEditor::TopBar_Save()
{
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
    
    if (ImGui::BeginMainMenuBar())
    {

        std::string s;
        s.reserve(200);

        for (int i = 0; i <180; i++)
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
                    auto current_level=GameStateManager::GetInstance()->GetCurrentLevel();
                    std::string cur_level_str=current_level->GetName();                    
                    Serializer::GetInstance()->SaveStage("json/" + cur_level_str + "/" + cur_level_str + ".txt");
                    
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

void MainEditor::ShowObjectList()
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
            m_pNewObject = obj;
        }
    }
    
    if (m_pNewObject)
    {        
        GLModel* model= m_pNewObject->GetModel();
        GLModel* NewModel = nullptr;
        Transform* transform = static_cast<Transform*>(m_pNewObject->FindComponent("Transform"));
        Sprite* sprite = static_cast<Sprite*>(m_pNewObject->FindComponent("Sprite"));        
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
                m_pNewObject->SetModelType(NewModel->GetModelType());                
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
                auto obj_id = m_pNewObject->GetID();
                GameObjectManager::GetInstance()->RemoveObject(obj_id);                
                m_pNewObject = nullptr;
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
                    if (/*ImGui::Button("Normal")&& */GLHelper::GetInstance()->GetLeftControlPressed())
                    {                                                
                        ChangeCurrentEditMode(EDIT_MODE::NORMAL);
                        ImGui::CloseCurrentPopup();
                    }
                    else if (/*ImGui::Button("TileEdit")*/GLHelper::GetInstance()->GetAltPressed())
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

// a. m_bSelectedObjByMouse -- true means an object is selected by mouse
// b. m_pTransByMouseSelect -- c가 있을 때만 유효해야 함
// c. m_pSelectedGoByMouse  -- a-c랑 동기화 필요
// d. m_bShowObjectWindowByClick -- 현재는 a랑 항상 값이 같아야 함
// e. m_bShowDeleteConf 

// 1. 모든 오브젝트와 마우스 좌표를 비교하여 충돌한 오브젝트가 있는지 없는지 판단하고 ... 등등
// 2. 선택된 오브젝트의 에디터 윈도우 보여주기
// 3-1. 일반모드: 선택된 오브젝트의 위치를 마우스와 동기화
// 3-2. 타일모드: 선택된 오브젝트를 삭제 할지 말지 판단하는 코드 실행 후 .. 삭제까지
void MainEditor::CheckSelecetedObjByMouse()
{    
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();    
    auto L_mouse_trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();
    auto screen_mouse_pos = GLHelper::GetInstance()->GetMouseCursorPosition();   
    auto HelperInst = GLHelper::GetInstance();
    
    for (const auto& obj : all_objs)
    {               
        Transform* trans = dynamic_cast<Transform*>(obj->FindComponent("Transform"));
        if (trans!=nullptr)
        {                                                                   
            if(!m_bSelectedObjByClick)
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
                        m_pSelectedObjByMouse = obj;
                        m_bSelectedObjByClick = true;                        
                    }
                }
            }
        }
    }          
    if (!L_mouse_trigger)
    {                        
        m_bSelectedObjByClick = false;
    }
}

void MainEditor::UniqueFunctionEachMode()
{
    auto HelperInst = GLHelper::GetInstance();
    auto screen_mouse_pos = GLHelper::GetInstance()->GetMouseCursorPosition();
    if (m_bSelectedObjByClick)
    {
        if (GetCurrentEditMode() == EDIT_MODE::NORMAL)
        {
            Wall* wall_comp=dynamic_cast<Wall*>(m_pTransByMouseSelect->GetOwner()->FindComponent(Wall::WallTypeName));
            if(wall_comp==nullptr)
                m_pTransByMouseSelect->SetPosition({ m_vWorldMousePos.x,m_vWorldMousePos.y });
        }            
        else if (GetCurrentEditMode() == EDIT_MODE::TILE)
        {
            auto a = HelperInst->GetLeftMouseTriggered();
            auto b = HelperInst->GetLeftControlPressed();
            if ( a&&b)
            {
                if (m_pTransByMouseSelect->GetOwner() != nullptr)
                {
                    auto obj_id = m_pTransByMouseSelect->GetOwner()->GetID();
                    std::string name = m_pTransByMouseSelect->GetOwner()->GetName();
                    m_bSelectedObjByClick = false;
                    Wall* wall_comp=dynamic_cast<Wall*>(m_pTransByMouseSelect->GetOwner()->FindComponent(Wall::WallTypeName));
                    /*if (wall_comp != nullptr)
                    {
                        glm::vec2 grid = wall_comp->GetScreenGrid();
                        TileEditor::GetInstance()->SetWallGridCoord(grid.x, grid.y, false);
                    }*/
                    GameObjectManager::GetInstance()->RemoveObject(obj_id, name);
                    /*m_pTransByMouseSelect = nullptr;
                    m_pSelectedObjByMouse = nullptr;*/
                    auto temp = TileEditor::GetInstance()->GetWallGrid();
                    HelperInst->ResetLeftMouseTriggered();
                }
            }
        }
    }
}

void MainEditor::ShowObjectInfoWindow()
{
    if (m_bSelectedObjByClick && !GLHelper::GetInstance()->GetLeftControlPressed())
    {
        ImGui::Begin((m_pSelectedObjByMouse->GetName() + std::to_string(m_pSelectedObjByMouse->GetID())).c_str(), &m_bSelectedObjByClick);
        GLModel* model = m_pSelectedObjByMouse->GetModel();
        GLModel* NewModel = nullptr;
        Transform* transform = static_cast<Transform*>(m_pSelectedObjByMouse->FindComponent("Transform"));
        Sprite* sprite = static_cast<Sprite*>(m_pSelectedObjByMouse->FindComponent("Sprite"));
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
                m_pSelectedObjByMouse->SetModelType(NewModel->GetModelType());
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
                auto obj_id = m_pSelectedObjByMouse->GetID();
                std::string name = m_pSelectedObjByMouse->GetName();
                GameObjectManager::GetInstance()->RemoveObject(obj_id, name);
                m_pSelectedObjByMouse = nullptr;
                m_bShowDeleteConf = false;
                m_bSelectedObjByClick = false;
            }
            else if (ImGui::Button("No"))
            {
                m_pSelectedObjByMouse = nullptr;
                m_bShowDeleteConf = false;
                m_bSelectedObjByClick = false;
            }
            ImGui::End();
        }
        if (ImGui::Button("Close Me"))
        {
            m_pSelectedObjByMouse = nullptr;
            m_bSelectedObjByClick = false;
        }
        ImGui::End();
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
    std::cout << m_vWorldMousePos.x << "," << m_vWorldMousePos.y << std::endl;
    if (!m_bCurWindowObjectList)
        CheckSelecetedObjByMouse();
    if (GetCurrentEditMode() == EDIT_MODE::TILE&& !m_bCurWindowObjectList)
    {        
        TileEditor::GetInstance()->Update();
    }

    ShowObjectList();
    UniqueFunctionEachMode();
    ShowObjectInfoWindow();

    TopBar_GameObject();
    TopBar_ChangeEditMode();
    TopBar_Save();
    TopBar_ShowGrid();
}