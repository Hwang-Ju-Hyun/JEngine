#pragma once
#include "header.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLHelper
{
public:
	SINGLE(GLHelper);
private:
	std::string m_strTitleName="";
	GLint m_giWidth=0;
	GLint m_giHeight = 0;
	GLFWwindow* m_ptrWindow=nullptr;
public:	
	bool Init(GLint _width, GLint _height, const std::string& _title);
	bool Exit();
public:
	GLFWwindow* GetWindow()const;
	GLint GetWindowWidth()const;
	GLint GetWindowHeight()const;
	std::string GetWindowTitleName()const;
public:
	static void KeyCallBack(GLFWwindow* _window,int _key, int _scancod, int _action, int _mod);
	static void MousePositionCallBack(GLFWwindow* _window, double _xpos, double _ypos);
	static void MousebuttonCallBack(GLFWwindow* _window, int _button, int _action, int _mod);
public:
	static void setup_event_callbacks();
private:
	GLboolean LeftMouse;
};

