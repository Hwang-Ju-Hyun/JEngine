#pragma once
#include "header.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameStateManager.h"
#include <iostream>
class GameStateManager;

class Engine
{
public:
	SINGLE(Engine);
public:
	bool Init(GLint _width, GLint _height, const std::string& _title);
	bool Update();
	bool Draw();
	bool Exit();
};

