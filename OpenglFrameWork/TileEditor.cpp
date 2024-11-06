#include "TileEditor.h"
#include "MainEditor.h"
#include "header.h"
#include "GLHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Serializer.h"
#include "Resource.h"
#include "GameStateManager.h"
#include "BaseLevel.h"
#include "ComponentManager.h"
#include "Wall.h"
#include <iostream>
#include <algorithm>


TileEditor::TileEditor()
{
    
}

TileEditor::~TileEditor()
{

}
   
void TileEditor::ShowAndSetCurrentWallFragile()
{
    bool show_cur_tile_confirm = false;
    ImGui::Begin("Fragile");    
    if (ImGui::TreeNode("TILE"))
    {
        if (ImGui::Button("Fragile"))
        {
            show_cur_tile_confirm = true;
            m_bCurFragileType = true;
            ImGui::TreePop();
        }
        if (ImGui::Button("No Fragile"))
        {
            show_cur_tile_confirm = true;
            m_bCurFragileType = false;
            ImGui::TreePop();
        }        
        if (!show_cur_tile_confirm)
            ImGui::TreePop();
    }
    ImGui::End();
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

glm::vec2 TileEditor::GetScreenGridByScreenPoint(glm::vec2 _pointPos)
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

int TileEditor::GetMaxXGrid() const
{
    return m_iMaxXGrid;
}

int TileEditor::GetMaxYGrid() const
{
    return m_iMaxYGrid;
}

bool TileEditor::GetExist() const
{
    return m_bExist;
}

std::vector<std::vector<bool>> TileEditor::GetWallGrid() const
{
    return m_vecWallGridCoord;
}

GameObject* TileEditor::FindObjectByGrid(glm::vec2 _grid)
{    
    auto all_comps=ComponentManager::GetInstance()->GetAllComponents();
    for (int i = 0; i < all_comps.size(); i++)
    {
        if (all_comps[i]->GetName() == Transform::TransformTypeName)
        {            
            Transform* trs = static_cast<Transform*>(all_comps[i]);
            if (trs->GetGridByScreenPos() == _grid)
            {
                return trs->GetOwner();
            }
        }
    } 
    return nullptr;
}

bool TileEditor::Init()
{        
    
    m_iMaxXGrid = 30;
    m_iMaxYGrid = 18;

    for (int i = 0; i < m_iMaxXGrid; i++)
    {
        std::vector<bool> temp;
        for (int j = 0; j < m_iMaxYGrid; j++)
        {            
            temp.push_back(false);
        }
        m_vecWallGridCoord.push_back(temp);
    }

    m_iNumberOfWallsCol  = 30;
    m_iNumberOfWallsRow  = 18;
    m_iWallWidth = window_width / m_iNumberOfWallsCol;
    m_iWallHeight = window_height / m_iNumberOfWallsRow;
    
    Serializer::GetInstance()->LoadStaticScreenGrid("json/Static/ScreenGrid.json");    
    std::vector<std::vector<bool>> stage_grid = TileEditor::GetInstance()->m_vecWallGridCoord;
    std::vector<std::vector<bool>> static_grid = TileEditor::GetInstance()->m_vecStaticWallGridCoord;
    for (int i = 0; i < stage_grid.size(); i++)
    {
        for (int j = 0; j < stage_grid[0].size(); j++)
        {
            m_vecWallGridCoord[i][j] = static_grid[i][j];
        }
    }
    

    return true;
}

void TileEditor::Update()
{
    auto current_level = GameStateManager::GetInstance()->GetCurrentLevel();
    std::string cur_level_str = current_level->GetName();
    Serializer::GetInstance()->LoadScreenGrid("json/" + cur_level_str + "/" + "Grid" + ".json");


    ShowAndSetCurrentTileTexture();
    ShowAndSetCurrentWallFragile();

    auto R_mouse_Trigger = GLHelper::GetInstance()->GetRightMouseTriggered();    
    
    glm::vec2 mouse_pos_screen = GLHelper::GetInstance()->GetMouseCursorPosition();        

    m_iScreenGridX = mouse_pos_screen.x / m_iWallWidth;
    m_iScreenGridY = mouse_pos_screen.y / m_iWallHeight;
    
    if (R_mouse_Trigger&&ImGui::IsMouseOutsideAnyWindow())
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
            
            wall_obj = new GameObject(Wall::WallTypeName, wall_last_id++);
            wall_obj->AddComponent(Transform::TransformTypeName, new Transform(wall_obj));
            wall_obj->AddComponent(Sprite::SpriteTypeName, new Sprite(wall_obj));      
            wall_obj->SetTexture(m_pCurrentTileTexture);            

            Transform* wall_trs = static_cast<Transform*>(wall_obj->FindComponent(Transform::TransformTypeName));
            
            glm::vec2 mouse_pos_world;
            mouse_pos_world=GetWorldPosbyScreenGrid(m_iWallWidth, m_iWallHeight, m_iScreenGridX, m_iScreenGridY);
            wall_trs->SetPosition({ mouse_pos_world.x,mouse_pos_world.y });
            wall_trs->SetScale({ m_iWallWidth, m_iWallHeight });            
            wall_obj->SetModelType(MODEL_TYPE::RECTANGLE);
            
            glm::mat3 wall_mat ={ wall_trs->GetScreenByWorld() };
            glm::vec2 wall_pos = { wall_mat[2][0],wall_mat[2][1] };
            glm::vec2 wall_grid = TileEditor::GetInstance()->GetScreenGridByScreenPoint(wall_pos);
            wall_trs->SetGridByScreenPos({ m_iScreenGridX,m_iScreenGridY });


            GLHelper::GetInstance()->ResetLeftMouseTriggered();
            SetWallGridCoord(m_iScreenGridX, m_iScreenGridY, true);
            Wall* wall_comp=nullptr;
            wall_comp = static_cast<Wall*>(wall_obj->AddComponent("Wall", new Wall(wall_obj)));
            wall_comp->SetFragile(m_bCurFragileType);

            Sprite* wall_spr = nullptr;
            wall_spr = static_cast<Sprite*>(wall_obj->FindComponent(Sprite::SpriteTypeName));

            if (wall_comp->GetFragile())
            {
                wall_spr->SetColor({ 0.f,1.f,0.f,1.f });
            }

            //Serializer::GetInstance()->SaveStage("json/" + cur_level_str + "/" + cur_level_str + ".txt");
            Serializer::GetInstance()->SaveScreenGrid("json/" + cur_level_str + "/" + "Grid" + ".json");
        }
    }      
}
