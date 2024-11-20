#include "TimeManager.h"
#include "GLFW/glfw3.h"
#include <iostream>

TimeManager::TimeManager()
{

}

TimeManager::~TimeManager()
{

}

bool TimeManager::Update()
{
	static double prev_time = glfwGetTime();
	double curr_time = glfwGetTime();

	static int count = 0;

	m_fDeltaTime =curr_time - prev_time;
	
	prev_time = curr_time;		
	count++;		

	return true;
}

float TimeManager::GetDeltaTime() const
{
	return m_fDeltaTime;
}
