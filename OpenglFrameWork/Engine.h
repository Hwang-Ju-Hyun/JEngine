#pragma once
#include "single.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameStateManager.h"
#include <string>

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

