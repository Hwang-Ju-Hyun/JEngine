#pragma once
#include "BaseComponent.h"

class GameObject;

class Collision :
    public BaseComponent
{
public:
    Collision(GameObject* _owner);
    virtual ~Collision()override;   
public:
    void Update();
public:
    virtual void LoadFromJson(const json& _str);
    virtual json SaveToJson(const json& _str);
public:
    static BaseRTTI* CreateCollisionComponent();
public:
    static constexpr const char* CollisionTypeName = "Collision";
};

