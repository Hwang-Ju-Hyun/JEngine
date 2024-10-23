#pragma once
#include "single.h"
#include <vector>
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
	BaseLevel* GetCurrentLevel()const;
};

