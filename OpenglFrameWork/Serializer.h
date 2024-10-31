#pragma once
#include "single.h"
#include <fstream>
#include <vector>

class GameObject;

class Serializer
{
public:
	SINGLE(Serializer);	
private:
	std::vector<std::string> m_vecJsonFile;	
public:
	GameObject* LoadGameObject(const std::string& _path);//TODO: ���߿� �Լ��̸� �� clear�ϰ� ������
	void SaveGameObject(const std::string& _path);
	int GetObjectSize(const std::string& _path);
public:
	GameObject* LoadWall(const std::string& _path);//TODO: ���߿� �Լ��̸� �� clear�ϰ� ������
	void SaveWall(const std::string& _path);
	int GetWallSize(const std::string& _path);
public:
	GameObject* LoadPlayer(const std::string& _path);
	void SavePlayer(const std::string& _path);
public:
	void LoadStage(const std::string& _path);
	void SaveStage(const std::string& _path);
public:
	void LoadJson(const std::string& _path);
	void SaveJson(const std::string& _path);
private:	
	std::string FindStageName(const std::string& _path);
	std::string SubstractObjectNameByFile(const std::string _path);
private:
	std::vector<std::string> m_vecFile;
	int m_iIndexForSave=0;
	std::string m_strRecentlyName = "";
};