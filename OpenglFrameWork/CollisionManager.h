#pragma once
#include "single.h"
#include <glm.hpp>
#include <vector>

class GameObject;
class Player;
class Collision;


class CollisionManager
{
public:
	SINGLE(CollisionManager);
private:
	GameObject* m_pPlayer = nullptr;
	GameObject* m_pBomb = nullptr;
private:
	bool IsOverLapRectAndRect(float _left1, float _right1, float _top1, float _bot1, float _left2, float _right2, float _top2, float _bot2);
	bool IsOverLapConvexsOfProj(float _amax, float _bmax, float _amin, float _bmin);
public:
	void HandlePosOnCollision_Rect_Rect(GameObject* _obj1, GameObject* _obj2);
	void HandlePosOnCollision_Rect_Circle(GameObject* _obj1, GameObject* _obj2);
	void HandlePosByCollisionCheck_Convex_Convex(GameObject* _obj1, GameObject* _obj2);
public:
	bool IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2);		
	bool IsCollisionConvexAndConvex(GameObject* _obj1, GameObject* _obj2);	
	bool IsCollisionCirlceAndCircle(GameObject* _obj1, GameObject* _obj2);
	bool IsCollisionCircleAndRect(GameObject* _obj1, GameObject* _obj2);
public:
	bool Init();
	bool Update();
public:
	typedef struct HitBox
	{
	public:
		GameObject* obj1;
		GameObject* obj2;
		bool result;
	public:
		HitBox(GameObject* _obj1, GameObject* _obj2, bool _result);
	};
private:
	std::vector<HitBox*> m_vecHitBoxes;
public:
	const std::vector<HitBox*> GetHitBoxes()const;
private:
	std::vector<GameObject*> m_vecCollision = {};
public:
	std::vector<GameObject*> GetCollsionVector()const;
	void AddCollisionObject(GameObject* _obj);
//Todo: 현재는 wall,player이런식으로 받지만 추후에 Layer로 수정해야함
private:
	std::vector<Collision*> m_vecWall_Col;
	std::vector<Collision*> m_vecPlayer_Col;
	std::vector<Collision*> m_vecBomb_Col;	
public:
	std::vector<Collision*> GetWallCollisionVec()const;
	std::vector<Collision*> GetPlayerCollisionVec()const;
	std::vector<Collision*> GetBombCollisionVec()const;
public:
	void AddBombToBombColVec(Collision* _bombCol);
};