#include "TileEditor.h"
#include "MainEditor.h"
#include "header.h"
#include "GLHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"

TileEditor::TileEditor()
{

}

TileEditor::~TileEditor()
{

}

void TileEditor::CaculateWallPosition()
{    
    int w = window_width;
    int h = window_height;    
    MainEditor::GetInstance()->m_vWorldMousePos = { MainEditor::GetInstance()->m_mScreenToWorldMat[2][0],MainEditor::GetInstance()->m_mScreenToWorldMat[2][1] };

    //1사분면
    if (((m_iScreenGridX * m_iWidth) + (m_iWidth / 2.f)) < w / 2
        && ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2.f)) < h / 2)
    {
        m_vWall.x = -w / 2 + ((m_iScreenGridX * m_iWidth) + (m_iWidth / 2));
        m_vWall.y = h / 2 - ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2));
    }
    //2사분면
    else if (((m_iScreenGridX * m_iWidth) + (m_iWidth / 2.f)) > w / 2
        && ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2.f)) < h / 2)
    {
        m_vWall.x = -(w / 2) + ((m_iScreenGridX * m_iWidth) + (m_iWidth / 2));
        m_vWall.y = (h / 2) - ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2));
    }
    //3사분면
    else if (((m_iScreenGridX * m_iWidth) + (m_iWidth / 2.f)) < w / 2
        && ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2.f)) > h / 2)
    {
        m_vWall.x = -(w / 2) + ((m_iScreenGridX * m_iWidth) + (m_iWidth / 2));
        m_vWall.y = (h / 2) - ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2));
    }
    else
    {
        m_vWall.x = -(w / 2) + ((m_iScreenGridX * m_iWidth) + (m_iWidth / 2));
        m_vWall.y = (h / 2) - ((m_iScreenGridY * m_iHeight) + (m_iHeight / 2));
    }
}

void TileEditor::Update()
{
    

    auto L_mouse_Trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();
    auto L_mouse_Released = GLHelper::GetInstance()->GetLeftMouseReleased();

    m_iNumberOfWalls = 30;

    m_iWidth = window_width / m_iNumberOfWalls;
    m_iHeight = window_height / m_iNumberOfWalls;

    glm::vec2 ScreenToMousePos = GLHelper::GetInstance()->GetMouseCursorPosition();
    glm::mat3 ScreenToWorldMat = GLHelper::GetInstance()->GetScreenToWorldMatFromMouse();

    int screen_grid_x = ScreenToMousePos.x / m_iWidth;
    int screen_grid_y = ScreenToMousePos.y / m_iHeight;

    GameObject* selected_obj = MainEditor::GetInstance()->m_pSelectedGameObject;

    if (L_mouse_Trigger)
    {
        if (!m_aWallGridCord[(int)screen_grid_x][(int)screen_grid_y])
        {
            static int id = 2;
            selected_obj = new GameObject("WALL", id++);
            selected_obj->AddComponent("Transform", new Transform(selected_obj));
            selected_obj->AddComponent("Sprite", new Sprite(selected_obj));
            Transform* trans = static_cast<Transform*>(selected_obj->FindComponent("Transform"));            

            CaculateWallPosition();

            trans->SetPosition({ m_vWall.x,m_vWall.y });
            trans->SetScale({ m_iWidth, m_iHeight });
            selected_obj->SetModelType(MODEL_TYPE::RECTANGLE);
            m_aWallGridCord[(int)screen_grid_x][(int)screen_grid_y] = true;
            GLHelper::GetInstance()->ResetLeftMouseTriggered();
        }
    }
}
