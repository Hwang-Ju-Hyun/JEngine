#pragma once

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <string>

class GLShader
{
public:
	GLShader();
	~GLShader();
private:
	GLuint m_iShaderPgmHandle = 0;
public:
	std::string ReadFile(const char* _filePath);
	GLuint AddShader(const char* _shaderCode, GLenum _shaderType);
	//void CompileShader(GLuint* _shader,const char* _vsCode, const char* _fsCode);
	//void CreateShaderProgramFromFiles(GLuint* _shader,const char* _vsPath, const char* _fsPath);
	bool CompileShader(const char* _vsCode, const char* _fsCode);
	bool CreateShaderProgramFromFiles(const char* _vsPath, const char* _fsPath);
public:
	GLuint GetShaderPgmHandle()const;
public:
	void Use();
	void UnUse();
};

