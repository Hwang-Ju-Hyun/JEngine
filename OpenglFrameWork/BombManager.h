#pragma once
#include "single.h"
#include <vector>

class GameObject;

class BombManager
{
public:
	SINGLE(BombManager);
private:
	std::vector<GameObject*> m_vecBombFragment;
public:
	void AddBombFragment(GameObject* _bombFrag);
	std::vector<GameObject*> GetAllBombFragmenets();
};