#pragma once
#include "BaseComponent.h"

class GameObject;

class Bomb :
    public BaseComponent
{
public:
    Bomb(GameObject* _owner);
    virtual ~Bomb()override;
private:
    float m_fExplodeTime = 0.f;
public:
    void AddExplodeTime(float _time);
    const float GetExplodeTime()const;
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override;
public:
    static BaseRTTI* CreateBombComponent();
public:
    static constexpr const char* BombTypeName = "Bomb";
};

