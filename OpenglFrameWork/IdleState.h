#pragma once
#include "BaseEnemyState.h"

class GameObject;

class IdleState :
    public BaseEnemyState
{
private:
    GameObject* m_pEnemy=nullptr;
public:
    IdleState();
    virtual ~IdleState()override;
public:
    virtual void Init()override;
    virtual void Update()override;
    virtual void Exit()override;
private:
    
public:
    static constexpr const char* IdleTypeName = "Idle";
};