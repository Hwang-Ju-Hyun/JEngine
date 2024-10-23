#pragma once
#include "BaseLevel.h"
#include <vector>

class GameObject;

class Stage02_Lvl :
    public BaseLevel
{
public:
    Stage02_Lvl(const std::string _name);
    virtual ~Stage02_Lvl()override;
public:
    virtual bool Init() override;
    virtual bool Update() override;
    virtual bool Exit() override;
private:
    std::vector<GameObject*> m_vecGameObject_Lvl02 = {};
    std::vector<GameObject*> m_vecWall_Lv02 = {};
};

