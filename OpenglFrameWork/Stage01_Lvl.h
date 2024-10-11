#pragma once
#include "BaseLevel.h"

class GameObject;


class Stage01_Lvl :
    public BaseLevel
{
public:
    Stage01_Lvl();
    virtual ~Stage01_Lvl()override;
public:
    virtual bool Init() override;
    virtual bool Update() override;
    virtual bool Exit() override;   
private:
    GameObject* temp_obj1 = nullptr;  
    GameObject* temp_obj2 = nullptr;
};

