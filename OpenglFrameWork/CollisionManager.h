#pragma once
#include "single.h"

class GameObject;

class CollisionManager
{
public:
	SINGLE(CollisionManager);
public:
	bool IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2);	
	bool IsCollisionRectAndTri(GameObject* _obj1, GameObject* _obj2);
};

