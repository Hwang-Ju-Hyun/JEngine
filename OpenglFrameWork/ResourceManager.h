#pragma once
#include "single.h"
#include <map>
#include <string>

class Resource;

class ResourceManager
{
public:
	SINGLE(ResourceManager);
private:
	std::map<std::string, Resource*> m_mapResource;
private:
	enum SELECT_EXTENSION
	{
		png,
		jpg,
		wav,
		mp3,
		ttf
	};
public:
	enum SELECT_EXTENSION GetExtension(const std::string& _extension)const;	
public:		
	Resource* GetAndLoad(const std::string& _resName, const std::string& _path);
	Resource* FindResource(const std::string& _resName);
public:
	std::map<std::string, Resource*> GetAllResources()const;
public:
	void RemoveAllRes();
};

