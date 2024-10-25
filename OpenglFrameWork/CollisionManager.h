#pragma once
#include "single.h"

class GameObject;
class Player;

class CollisionManager
{
public:
	SINGLE(CollisionManager);
private:
	GameObject* m_pPlayer = nullptr;	
public:
	bool IsOverLapRectAndRect(float _left1, float _right1, float _top1, float _bot1, float _left2, float _right2, float _top2, float _bot2);
public:
	bool IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2);	
	bool IsCollisionRectAndTri(GameObject* _obj1, GameObject* _obj2);
private:
	//나중에 GLHelper로 옮기자
	bool IsRectangleInsideTriangle(GameObject* _obj1, GameObject* _obj2);
public:
	bool Init();
	bool Update();
};

