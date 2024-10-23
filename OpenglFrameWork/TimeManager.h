#pragma once
#include "single.h"
#include "GLFW/glfw3.h"
class TimeManager
{
public:
	SINGLE(TimeManager);	
private:	
	double m_fDeltaTime = -1;
public:
	bool Update();
public:
	float GetDeltaTime()const;
};

