#pragma once
#include "single.h"
#include <fstream>
#include <vector>
#include "BaseRTTI.h"

class GameObject;

class Serializer
{
public:
	SINGLE(Serializer);	
private:
	std::vector<std::string> m_vecJsonFile;			
public:
	void LoadStage(const std::string& _path);
	void SaveStage(const std::string& _path);
public:
	GameObject* LoadJson(const std::string& _path,bool _IsPrefabs=false);
	void SaveJson(const std::string& _path);
public:
	void LoadScreenGrid(const std::string& _path);
	void SaveScreenGrid(const std::string& _path);
public:	
	GameObject* CreateObjectFromJson(json _item);
private:	
	std::string FindStageName(const std::string& _path);
	std::string SubstractObjectNameByFile(const std::string _path);
private:
	std::vector<std::string> m_vecFile;
	int m_iIndexForSave=0;
	std::string m_strRecentlyName = "";
};