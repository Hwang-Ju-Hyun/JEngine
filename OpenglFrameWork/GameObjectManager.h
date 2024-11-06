#pragma once
#include "single.h"
#include <vector>
#include <string>

class GameObject;
class Player;

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
	void RemoveObject(int _id);
	void RemoveObject(int _id, std::string _name);
	void RemoveObjectsByName(std::string _name);
public:
	bool Init();
	bool Update();
	bool Draw();
private:
	std::vector<Player*> m_vecPlayerCompVec;
public:
	void AddPlayerToPlayerVec(Player* _playerComp);
	std::vector<Player*> GetPlayerCompVec()const;
};