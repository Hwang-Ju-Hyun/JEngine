#include "TileEditor.h"
#include "MainEditor.h"
#include "header.h"
#include "GLHelper.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "Sprite.h"

TileEditor::TileEditor()
{

}

TileEditor::~TileEditor()
{

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

void TileEditor::Update()
{    
    auto L_mouse_Trigger = GLHelper::GetInstance()->GetLeftMouseTriggered();    
    
    glm::vec2 ScreenToMousePos = GLHelper::GetInstance()->GetMouseCursorPosition();    

    int NumberOfWalls = 30;

    int WallWidth = window_width / NumberOfWalls;
    int WallHeight = window_height / NumberOfWalls;


    int ScreenGridX = ScreenToMousePos.x / WallWidth;
    int ScreenGridY = ScreenToMousePos.y / WallHeight;

    if (L_mouse_Trigger)
    {
        if (!m_aWallGridCord[ScreenGridX][ScreenGridY])
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
            
            Transform* trans = static_cast<Transform*>(wall_obj->FindComponent("Transform"));
            
            glm::vec2 wall;

            wall=GetWorldPosbyScreenGrid(WallWidth,WallHeight,ScreenGridX,ScreenGridY);

            trans->SetPosition({ wall.x,wall.y });
            trans->SetScale({ WallWidth, WallHeight });
            wall_obj->SetModelType(MODEL_TYPE::RECTANGLE);

            GLHelper::GetInstance()->ResetLeftMouseTriggered();            
            m_aWallGridCord[(int)ScreenGridX][(int)ScreenGridY] = true;
        }
    }
}
