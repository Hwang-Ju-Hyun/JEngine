#pragma once
#include "single.h"
#include <map>
#include <string>

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