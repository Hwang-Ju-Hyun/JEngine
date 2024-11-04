#include "GLApp.h"
#include "GLHelper.h"
#include "GLShader.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "TileEditor.h"
#include "MainEditor.h"

GLApp::GLApp()
{
}

GLApp::~GLApp()
{	
}

bool GLApp::Init()
{	
	GLint width = GLHelper::GetInstance()->GetWindowWidth();
	GLint height = GLHelper::GetInstance()->GetWindowHeight();
	glViewport(0, 0, width, height);			

	std::vector<std::pair<std::string/*vertex*/, std::string/*fragment*/>> vecShader;

	vecShader.push_back({m_VertexShader,m_FragmentShader });
	Init_Shader(vecShader);// ToDo: 함수역할과 함수 이름이 언행불일치하다
		

	return true;
}

bool GLApp::Update()
{
	return true;
}


void GLApp::Init_Shader(const std::vector<std::pair<std::string, std::string>>& _vecShader)
{
	for (int i = 0; i < _vecShader.size(); i++)
	{
		GLShader shader;
		if (shader.CreateShaderProgramFromFiles(_vecShader[i].first.c_str(), _vecShader[i].second.c_str()))
		{
			ShaderManager::GetInstance()->AddShader(shader);
		}				
	}
}

GLuint GLApp::GetShaderHandle() const
{
	return m_iShader;
}

bool GLApp::Exit()
{	
	return false;
} 