#pragma once
#include "single.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <iostream>

class GameObject;

class GLApp
{
public:
	SINGLE(GLApp);	
public:
	bool Init();
	bool Update();	
	bool Exit();
private:
	const std::string m_VertexShader   = "shader.vert";
	const std::string m_FragmentShader = "shader.frag";
	GLuint m_iShader;
public:
	void Init_Shader(const std::vector<std::pair<std::string, std::string>>& _vecShader);
	GLuint GetShaderHandle()const;	
};

