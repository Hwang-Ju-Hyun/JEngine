#pragma once
#include "header.h"

class BaseLevel;

class GameStateManager
{
public:
	SINGLE(GameStateManager);
private:
	BaseLevel* m_pCurrentLevel = nullptr;
	BaseLevel* m_pPreviousLevel = nullptr;
public:
	bool Init();
	bool Update();
	bool Exit();
public:
	bool ChangeLevel(BaseLevel* _newLevel);
};

