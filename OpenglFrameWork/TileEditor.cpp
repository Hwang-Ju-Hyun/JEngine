#include "TileEditor.h"
#include "MainEditor.h"
#include "header.h"
#include "GLHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Resource.h"
#include "Wall.h"
#include <iostream>
#include <algorithm>


TileEditor::TileEditor()
{
    
}

TileEditor::~TileEditor()
{

}
   
void TileEditor::ShowAndSetCurrentTileTexture()
{   
    bool show_cur_tile_confirm = false;
    ImGui::Begin("Texture");    
    auto all_resources = ResourceManager::GetInstance()->GetAllResources();
    if (ImGui::TreeNode("TILE"))
    {
        for (auto iter = all_resources.begin(); iter != all_resources.end(); iter++)
        {
            if (ImGui::Button(iter->first.c_str()))
            {
                show_cur_tile_confirm = true;
                m_pCurrentTileTexture = (TextureResource*)iter->second;
                ImGui::TreePop();            
            }            
        }
        if (ImGui::Button("No Texture"))
        {
            show_cur_tile_confirm = true;
            m_pCurrentTileTexture = nullptr;
            ImGui::TreePop();
        }
        if(!show_cur_tile_confirm)
            ImGui::TreePop();
    }    
    ImGui::End();
}   
    
glm::vec2 TileEditor::GetWorldPosbyScreenGrid(int _width, int _height, int _gridX, int _gridY)
{   
    int w = window_width;
    int h = window_height;

    glm::vec2 wall;

    wall.x = -w / 2 + ((_gridX * _width) + (_width  / 2));
    wall.y = h / 2 - ((_gridY * _height) + (_height / 2));  

    return wall;
}

glm::vec2 TileEditor::GetScreenGridByPoint(glm::vec2 _pointPos)
{        
    int WallWidth = window_width / m_iNumberOfWallsCol;
    int WallHeight = window_height / m_iNumberOfWallsRow;

    int ScreenGridX = _pointPos.x / WallWidth;
    int ScreenGridY = _pointPos.y / WallHeight;

    return glm::vec2{ ScreenGridX,ScreenGridY };
}

void TileEditor::SetWallGridCoord(int _x, int _y,bool _flag)
{
    m_vecWallGridCoord[_x][_y] = _flag;
    SetScreenGridX(_x);
    SetScreenGridX(_y);
    SetExist(_flag);
}

int TileEditor::GetNumberOfWallWidth() const
{
    return m_iNumberOfWallsCol;
}

int TileEditor::GetNumberOfWallHeight() const
{
    return m_iNumberOfWallsRow;
}

void TileEditor::SetWallWidth(int _width)
{
    m_iWallWidth = _width;
}

void TileEditor::SetWallHeight(int _height)
{
    m_iWallHeight = _height;
}

void TileEditor::SetScreenGridX(int _gridX)
{
    m_iScreenGridX = _gridX;
}

void TileEditor::SetScreenGridY(int _gridY)
{
    m_iScreenGridY = _gridY;
}

void TileEditor::SetExist(bool _exist)
{
    m_bExist = _exist;
}

int TileEditor::GetWallWidth() const
{
    return m_iWallWidth;
}

int TileEditor::GetWallHeight() const
{
    return m_iWallHeight;
}

int TileEditor::GetScreenGridX() const
{
    return m_iScreenGridX;
}

int TileEditor::GetScreenGridY() const
{
    return m_iScreenGridY;
}

bool TileEditor::GetExist() const
{
    return m_bExist;
}

std::vector<std::vector<bool>> TileEditor::GetWallGrid() const
{
    return m_vecWallGridCoord;
}

bool TileEditor::Init()
{        
    m_iMaxXGrid = 15;
    m_iMaxYGrid = 15;

    for (int i = 0; i < m_iMaxXGrid; i++)
    {
        std::vector<bool> temp;
        for (int j = 0; j < m_iMaxYGrid; j++)
        {            
            temp.push_back(false);
        }
        m_vecWallGridCoord.push_back(temp);
    }

    m_iNumberOfWallsCol  = 15;
    m_iNumberOfWallsRow  = 10;
    m_iWallWidth = window_width / m_iNumberOfWallsCol;
    m_iWallHeight = window_height / m_iNumberOfWallsRow;    
    return true;
}

void TileEditor::Update()
{
    ShowAndSetCurrentTileTexture();
    auto L_mouse_Trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();    
    
    glm::vec2 mouse_pos_screen = GLHelper::GetInstance()->GetMouseCursorPosition();        

    m_iScreenGridX = mouse_pos_screen.x / m_iWallWidth;
    m_iScreenGridY = mouse_pos_screen.y / m_iWallHeight;
    
    if (L_mouse_Trigger)
    {        
        MainEditor::GetInstance()->m_bSelectedObjByClick;
        if (!m_vecWallGridCoord[m_iScreenGridX][m_iScreenGridY])
        {                  
            auto all_objs = GameObjectManager::GetInstance()->GetAllObject();

            int wall_last_id=-1;

            for (int i = 0; i <all_objs.size(); i++)
            {       
                if (all_objs[i]->GetName() == "Wall")
                {
                    wall_last_id = all_objs[i]->GetID()+1;
                }
            }        
            GameObject* wall_obj=nullptr;
            
            wall_obj = new GameObject("Wall", wall_last_id++);
            wall_obj->AddComponent("Transform", new Transform(wall_obj));            
            wall_obj->AddComponent("Sprite", new Sprite(wall_obj));            
            wall_obj->SetTexture(m_pCurrentTileTexture);

            Transform* trans = static_cast<Transform*>(wall_obj->FindComponent("Transform"));
            
            glm::vec2 wall_grid;

            wall_grid=GetWorldPosbyScreenGrid(m_iWallWidth, m_iWallHeight, m_iScreenGridX, m_iScreenGridY);

            trans->SetPosition({ wall_grid.x,wall_grid.y });
            trans->SetScale({ m_iWallWidth, m_iWallHeight });
            wall_obj->SetModelType(MODEL_TYPE::RECTANGLE);

            GLHelper::GetInstance()->ResetLeftMouseTriggered();            
            m_vecWallGridCoord[(int)m_iScreenGridX][(int)m_iScreenGridY] = true;
            Wall* wall_comp=nullptr;
            wall_comp = static_cast<Wall*>(wall_obj->AddComponent("Wall", new Wall(wall_obj)));
            wall_comp->SetScreeGrid(wall_grid);
            wall_comp->SetFragile(false);
            wall_comp->SetExist(true);

            std::cout << m_iScreenGridX << "," << m_iScreenGridY << std::endl;

        }
    }
}
