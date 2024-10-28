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
private:
	bool IsOverLapRectAndRect(float _left1, float _right1, float _top1, float _bot1, float _left2, float _right2, float _top2, float _bot2);
private:
	void 
public:
	bool IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2);		
	bool IsCollisionConvexAndConvex(GameObject* _obj1, GameObject* _obj2);	
	bool IsCollisionCirlceAndCircle(GameObject* _obj1, GameObject* _obj2);
public:
	bool Init();
	bool Update();
};

