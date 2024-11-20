#pragma once
#include "BaseEnemyState.h"
class IdleState :
    public BaseEnemyState
{
public:
    IdleState();
    virtual ~IdleState()override;
public:
    virtual void Init()override;
    virtual void Update()override;
    virtual void Exit()override;
};