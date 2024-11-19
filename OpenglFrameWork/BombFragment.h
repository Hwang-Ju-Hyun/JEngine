#pragma once
#include "BaseComponent.h"

class GameObject;

class BombFragment :
    public BaseComponent
{
public:
    BombFragment(GameObject* _owner);
    virtual ~BombFragment()override;
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override;
public:
    static BaseRTTI* CreateBombComponent();
public:
    static constexpr const char* BombTypeName = "BombFragment";
};

