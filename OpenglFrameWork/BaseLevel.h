#pragma once
#include <string>
#include <vector>
#include "BaseRTTI.h"

class BaseLevel
{
public:
	BaseLevel() = delete;
	BaseLevel(const std::string _name);
	virtual ~BaseLevel();
public:
	virtual bool Init()=0;
	virtual bool Update()=0;
	virtual bool Exit()=0;
private:
	std::string m_strName;
public:	
	void SetName(const std::string& _strName);
	const std::string& GetName()const;	
};