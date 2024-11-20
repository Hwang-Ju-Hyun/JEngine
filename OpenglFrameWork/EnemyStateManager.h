#pragma once
#include "single.h"

class BaseEnemyState;

class EnemyStateManager
{
public:
	SINGLE(EnemyStateManager);
private:
	BaseEnemyState* m_pCurState=nullptr;
	BaseEnemyState* m_pPrevState = nullptr;
public:
	virtual void Init();
	virtual void Update();
	virtual void Exit();
public:
	void ChangeState(BaseEnemyState* _state);
	bool ShouldExit();
};

