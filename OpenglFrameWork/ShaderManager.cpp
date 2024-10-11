#include "ShaderManager.h"
#include "GLShader.h"
ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::AddShader(GLShader _shader)
{
	m_vecShader.push_back(_shader);
}

std::vector<GLShader> ShaderManager::GetAllShader() const
{
	return m_vecShader;
}
