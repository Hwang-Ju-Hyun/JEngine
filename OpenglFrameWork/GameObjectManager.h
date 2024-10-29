#pragma once
#include "single.h"
#include <vector>
#include <string>

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
	void RemoveObject(unsigned int _id);
	void RemoveObject(unsigned int _id, std::string _name);
	void RemoveObjectsByName(std::string _name);
public:

public:
	bool Init();
	bool Update();
	bool Draw();
};

