#pragma once
#include "header.h"

class GameObject;

class GameObjectManager
{
public:
	SINGLE(GameObjectManager);
private:
	std::vector<GameObject*> m_vecGameObject;
public:
	GameObject* AddObject(GameObject* _obj);
	GameObject* FindObject(const std::string& _objName)const;
	GameObject* GetLastObject()const;
	std::vector<GameObject*> GetAllObject()const;
	void RemoveAllObjects();
public:
	bool Init();
	bool Update();
	bool Draw();
};

