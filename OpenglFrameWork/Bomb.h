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
    bool m_bIsExplode = false;
    float m_fExplodeTime = 0.f;
public:
    void SetIsExplode(bool _explode);
    bool GetIsExplode()const;
    void SetExplodeTime(float _time);
    void AddExplodeTime(float _time);
    const float GetExplodeTime()const;
public:
    void Update();
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override;
public:
    static BaseRTTI* CreateBombComponent();
public:
    static constexpr const char* BombTypeName = "Bomb";
};