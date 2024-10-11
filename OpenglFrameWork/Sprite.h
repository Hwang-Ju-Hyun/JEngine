#pragma once
#include "BaseComponent.h"
class Sprite :
    public BaseComponent
{
public:
    Sprite(GameObject* _owner);
    virtual ~Sprite()override;
private:
    bool DrawTexture = false;    
public:
    virtual void Update() override;
};

