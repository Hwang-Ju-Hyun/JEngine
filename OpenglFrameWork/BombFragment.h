#pragma once
#include "BaseComponent.h"

class GameObject;

class BombFragment :
    public BaseComponent
{
public:
    BombFragment(GameObject* _owner);
    virtual ~BombFragment()override;
private:
    float m_fExplodingTime = 0.f;
public:
    void SetExplodingTime(float _time);
    float GetExplodingTime()const;
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override;
public:
    static BaseRTTI* CreateBombFragComponent();
public:
    static constexpr const char* BombFragmentTypeName = "BombFragment";
};

