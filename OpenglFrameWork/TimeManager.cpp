#include "TimeManager.h"
#include "GLFW/glfw3.h"

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

	m_fDeltaTime =curr_time - prev_time;
	
	prev_time = curr_time;		

	return true;
}

float TimeManager::GetDeltaTime() const
{
	return m_fDeltaTime;
}
