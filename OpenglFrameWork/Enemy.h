#pragma once
#include "BaseComponent.h"
class Enemy :
    public BaseComponent
{
public:
    Enemy(GameObject* _owner);
    virtual ~Enemy()override;
private:
    int m_iHP = 0;
    int m_iMaxBombCnt = 0;
    int m_iCurBombCnt = 0;
public:
    void SetHp(int _hp);
    int GetHp()const;
    void SetMaxBombCnt(int _cnt);
    void AddMaxBombCnt(int _cnt);
    int GetMaxBombCnt()const;
public:
    virtual void Update()override;
public:
    virtual void EventCollision(Collision* _pOther)override;
public:
    static constexpr const char* EnemyTypeName = "Enemy";
public:
    virtual void LoadFromJson(const json& _str);
    virtual json SaveToJson(const json& _str);
public:
    static BaseRTTI* CreateEnemyComponent();
};

