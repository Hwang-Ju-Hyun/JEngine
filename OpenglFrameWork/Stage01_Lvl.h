#pragma once
#include "BaseLevel.h"
#include <vector>

class GameObject;

class Stage01_Lvl :
    public BaseLevel
{
public:
    Stage01_Lvl(std::string _name);
    virtual ~Stage01_Lvl()override;
public:
    virtual bool Init() override;
    virtual bool Update() override;
    virtual bool Exit() override;   
    
private:    
    GameObject* m_pPlayer = nullptr;
    std::vector<GameObject*> m_vecGameObject_Lvl01 = {};
    std::vector<GameObject*> m_vecWall_Lv01 = {};    
};