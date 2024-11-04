#pragma once
#include "single.h"
#include <string>

class GameObject;

class Prefabs
{
public:
	SINGLE(Prefabs);
public:
	void CreateBombs(const std::string& _path, GameObject* _bombOwner);
};