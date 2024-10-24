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

TileEditor::TileEditor()
{
    m_iNumberOfWalls = 30;
}

TileEditor::~TileEditor()
{

}

    
int TileEditor::GetNumberOfWalls() const
{
    return m_iNumberOfWalls;
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

    wall.x = -w / 2 + ((_gridX * _width) + (_width / 2));
    wall.y = h / 2 - ((_gridY * _height) + (_height / 2));  

    return wall;
}

glm::vec2 TileEditor::GetScreenGridByMousePos(glm::vec2 _mousePos)
{        
    int WallWidth = window_width / m_iNumberOfWalls;
    int WallHeight = window_height / m_iNumberOfWalls;

    int ScreenGridX = _mousePos.x / WallWidth;
    int ScreenGridY = _mousePos.y / WallHeight;

    return glm::vec2{ ScreenGridX,ScreenGridY };
}

void TileEditor::SetWallGridCoord(int x, int y,bool _flag)
{
    m_bWallGridCoord[x][y] = _flag;
}

void TileEditor::Update()
{    
    ShowAndSetCurrentTileTexture();
    auto L_mouse_Trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();    
    
    glm::vec2 mouse_pos_screen = GLHelper::GetInstance()->GetMouseCursorPosition();    
    
    int WallWidth = window_width / m_iNumberOfWalls;
    int WallHeight = window_height / m_iNumberOfWalls;


    int ScreenGridX = mouse_pos_screen.x / WallWidth;
    int ScreenGridY = mouse_pos_screen.y / WallHeight;

    if (L_mouse_Trigger)
    {
        if (!m_bWallGridCoord[ScreenGridX][ScreenGridY])
        {                  
            auto all_objs = GameObjectManager::GetInstance()->GetAllObject();

            int wall_last_id=-1;

            for (int i = 0; i <all_objs.size(); i++)
            {       
                if (all_objs[i]->GetName() == "WALL")
                {
                    wall_last_id = all_objs[i]->GetID()+1;
                }
            }            
            GameObject* wall_obj=nullptr;
            
            wall_obj = new GameObject("WALL", wall_last_id++);
            wall_obj->AddComponent("Transform", new Transform(wall_obj));
            wall_obj->AddComponent("Sprite", new Sprite(wall_obj));            
            wall_obj->SetTexture(m_pCurrentTileTexture);


            Transform* trans = static_cast<Transform*>(wall_obj->FindComponent("Transform"));
            
            glm::vec2 wall;

            wall=GetWorldPosbyScreenGrid(WallWidth,WallHeight,ScreenGridX,ScreenGridY);

            trans->SetPosition({ wall.x,wall.y });
            trans->SetScale({ WallWidth, WallHeight });
            wall_obj->SetModelType(MODEL_TYPE::RECTANGLE);

            GLHelper::GetInstance()->ResetLeftMouseTriggered();            
            m_bWallGridCoord[(int)ScreenGridX][(int)ScreenGridY] = true;
        }
    }
}
