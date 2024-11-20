#pragma once
#include "single.h"
#include <string>

class GameObject;
class Bomb;
class BombFragment;

class Prefabs
{
public:
	SINGLE(Prefabs);
public:
	Bomb* CreateBombs(const std::string& _path, GameObject* _bombOwner);
	BombFragment* CreateBombFragment(const std::string& _path,Bomb* _bomb);
};