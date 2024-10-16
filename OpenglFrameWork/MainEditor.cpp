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
                    auto id = GameObjectManager::GetInstance()->GetAllObject().size();                    
                    m_ptrSelectedGameObject = new GameObject("tempObject", id++);
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
                    Serializer::GetInstance()->SaveWall("json/GameObject/Wall.json");
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
            if (NewModel != nullptr)
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

bool MainEditor::IsMouseInsideObject(GameObject* _obj)
{
    Transform* trs = dynamic_cast<Transform*>(_obj->FindComponent("Transform"));
    if (trs == nullptr)
        return false;

    float Left = trs->GetPosition().x - trs->GetScale().x / 2.f;
    float Right = trs->GetPosition().x + trs->GetScale().x / 2.f;
    float Top = trs->GetPosition().y + trs->GetScale().y / 2.f;
    float Bottom = trs->GetPosition().y - trs->GetScale().y / 2.f;

    if (m_vWorldMousePos.x >= Left && m_vWorldMousePos.x <= Right
        && m_vWorldMousePos.y <= Top && m_vWorldMousePos.y >= Bottom)
    {
        return true;
    }
    return false;
}

void MainEditor::EditMapMode()//Not working perfectly
{        
    auto L_mouse_Trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();
    auto L_mouse_Released = GLHelper::GetInstance()->GetLeftMouseReleased();

    int number_of_walls = 30;
    int wall_width = window_width / number_of_walls;
    int wall_height = window_height / number_of_walls;

    m_mScreenToMousePos = GLHelper::GetInstance()->GetMouseCursorPosition();
    m_mScreenToWorldMat = GLHelper::GetInstance()->GetScreenToWorldMatFromMouse();

    int screen_grid_x = screen_mouse_pos.x / wall_width;
    int screen_grid_y = screen_mouse_pos.y / wall_height;

    if (L_mouse_Trigger)
    {
        

        if (!m_aWallGridCord[(int)screen_grid_x][(int)screen_grid_y])
        {
            static int id = 2;
            m_ptrSelectedGameObject = new GameObject("WALL", id++);
            m_ptrSelectedGameObject->AddComponent("Transform", new Transform(m_ptrSelectedGameObject));
            m_ptrSelectedGameObject->AddComponent("Sprite", new Sprite(m_ptrSelectedGameObject));
            Transform* trans = static_cast<Transform*>(m_ptrSelectedGameObject->FindComponent("Transform"));                                     
            glm::vec2 wall;

            CaculateWallPosition(&wall);

            trans->SetPosition({ wall.x,wall.y });
            trans->SetScale({ wall_width, wall_height });
            m_ptrSelectedGameObject->SetModelType(MODEL_TYPE::RECTANGLE);
            m_aWallGridCord[(int)screen_grid_x][(int)screen_grid_y] = true;
            GLHelper::GetInstance()->ResetLeftMouseTriggered();
        }
    }    
}

void MainEditor::CaculateWallPosition(glm::vec2* _wall)
{   
    int w = window_width;
    int h = window_height;

    m_vWorldMousePos = { m_mScreenToWorldMat[2][0],m_mScreenToWorldMat[2][1] };
  

    //1사분면
    if (((screen_grid_x * wall_width) + (wall_width / 2.f)) < window_width / 2
        && ((screen_grid_y * wall_height) + (wall_height / 2.f)) < window_height / 2)
    {
        _wall->x = -w / 2 + ((screen_grid_x * wall_width) + (wall_width / 2));
        _wall->y = h / 2 - ((screen_grid_y * wall_height) + (wall_height / 2));
    }
    //2사분면
    else if (((screen_grid_x * wall_width) + (wall_width / 2.f)) > window_width / 2
        && ((screen_grid_y * wall_height) + (wall_height / 2.f)) < window_height / 2)
    {
        _wall->x = -(w / 2) + ((screen_grid_x * wall_width) + (wall_width / 2));
        _wall->y = (h / 2) - ((screen_grid_y * wall_height) + (wall_height / 2));
    }
    //3사분면
    else if (((screen_grid_x * wall_width) + (wall_width / 2.f)) < window_width / 2
        && ((screen_grid_y * wall_height) + (wall_height / 2.f)) > window_height / 2)
    {
        _wall->x = -(w / 2) + ((screen_grid_x * wall_width) + (wall_width / 2));
        _wall->y = (h / 2) - ((screen_grid_y * wall_height) + (wall_height / 2));
    }
    else
    {
        _wall->x = -(w / 2) + ((screen_grid_x * wall_width) + (wall_width / 2));
        _wall->y = (h / 2) - ((screen_grid_y * wall_height) + (wall_height / 2));
    }     
}

void MainEditor::SelectedObjectByMouse()
{    
    auto all_objs = GameObjectManager::GetInstance()->GetAllObject();    
    auto L_mouse_trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();        
    for (const auto& obj : all_objs)
    {       
        if (!m_bSelectedObjByMouse&&IsMouseInsideObject(obj))
        {
            m_pTransByMouseSelect = static_cast<Transform*>(obj->FindComponent("Transform"));
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
    if (m_bSelectedObjByMouse)
    {                       
        std::cout << m_pTransByMouseSelect->m_pOwner->GetID() << std::endl;       
        m_pTransByMouseSelect->SetPosition({m_vWorldMousePos.x,m_vWorldMousePos.y});
    }    
    if (!L_mouse_trigger)
    {                        
        m_bSelectedObjByMouse = false;        
    }
}



void MainEditor::Update()
{
    auto ScreenToWorld = GLHelper::GetInstance()->GetScreenToWorldMatFromMouse();
    m_vWorldMousePos = { ScreenToWorld[2][0],ScreenToWorld[2][1] };    

    TopBar_GameObject();
    TopBar_Save();
    //SelectedObjectByMouse();
    EditMapMode(); //Not working perfectly
    SelectedObjectWindow();
}