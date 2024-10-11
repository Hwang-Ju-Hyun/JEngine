#pragma once
#include "header.h"
#include <map>

class BaseRTTI;

class Registry
{
public:
	SINGLE(Registry);
private:
	std::map<std::string, BaseRTTI* (*)()> m_mapRttiMap;
public:
	BaseRTTI* FindOrCreate(const std::string& _type);
};