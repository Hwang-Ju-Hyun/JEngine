#include "GLShader.h"
#include <fstream>
#include <iostream>

GLShader::GLShader()
{
}

GLShader::~GLShader()
{
}

std::string GLShader::ReadFile(const char* _filePath)
{
	std::string content;
	std::ifstream fileStream(_filePath, std::ios::in);
	while (!fileStream.is_open())
	{		
		std::cout << "Error : Failed to read " << _filePath << " file - GLShader::ReadFile" << std::endl;
		return " ";
	}
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + '\n');
	}
	fileStream.close();
	return content;
}

GLuint GLShader::AddShader(const char* _shaderCode, GLenum _shaderType)
{
	//glCreateShader :  셰이더 프로그램과 연결할 셰이더를 생성하며 그 ID를 반환(핸들값)
	GLuint new_shader = glCreateShader(_shaderType); //쉐이더파일의 id값
	const GLchar* code[1];
	code[0] = _shaderCode;

	//glShaderSource : gpu가 인식할 수 있도록 소스코드를 넣어주는 느낌
	glShaderSource(new_shader, 1, code,NULL);

	GLint result = 0;
	GLchar err_log[1024] = { 0, };

	//핸들값을 토대로 쉐이더를 컴파일
	glCompileShader(new_shader);

	//쉐이더 컴파일 상태 확인
	glGetShaderiv(new_shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		//Shader의 Compile 실패 Log 확인
		glGetShaderInfoLog(new_shader, sizeof(err_log), NULL, err_log);		
		std::cout << "Error compiling the" << _shaderType << " shader : " << err_log;
		std::cout << " - GLShader::AddFile" << std::endl;
		return -1;
	}

	return new_shader;
}

bool GLShader::CompileShader(const char* _vsCode, const char* _fsCode)
{
	GLuint vs;
	GLuint fs;

	//쉐이더 프로그램이란 여러 shader를 결합한 마지막 연결된 버전입니다.
	m_iShaderPgmHandle = glCreateProgram();
	if (m_iShaderPgmHandle<=0)
	{
		std::cout << "Error : Cannot create shader program. -GLShader::CompileShader" << std::endl;
		return false;
	}
	vs = AddShader(_vsCode, GL_VERTEX_SHADER);
	fs = AddShader(_fsCode, GL_FRAGMENT_SHADER);

	//glAttachShader() - Shader Program에 사용할 Shader 등록(Vertex, Fragment 필수)
	glAttachShader(m_iShaderPgmHandle, vs);
	glAttachShader(m_iShaderPgmHandle, fs);

	GLint result = 0;
	GLchar err_log[1024] = { 0 };

	//셰이더 파일들을 링크하고 GPU에서 실행할 실행 파일을 만드는 OpenGL 함수.
	glLinkProgram(m_iShaderPgmHandle);
	//해당 연결된 프로그램이 옳게 문제없이 만들어졌는지 확인 (링킹이 올바르게 되었는지 체크)
	glGetProgramiv(m_iShaderPgmHandle, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_iShaderPgmHandle, sizeof(err_log), NULL, err_log);
		std::cout << "Error Linking Program : " << err_log;
		std::cout << " - GLShader::CompileShader" << std::endl;
		return false;
	}
	return true;
}

bool GLShader::CreateShaderProgramFromFiles(const char* _vsPath, const char* _fsPath)
{
	std::string vsFile = ReadFile(_vsPath);
	std::string fsFile = ReadFile(_fsPath);
	
	const char* vsCode = vsFile.c_str();
	const char* fsCode = fsFile.c_str();	
	
	if (!CompileShader(vsCode, fsCode))
		return false;
	return true;
}

GLuint GLShader::GetShaderPgmHandle() const
{
	return m_iShaderPgmHandle;
}


void GLShader::Use()
{
	if (m_iShaderPgmHandle>0)
		glUseProgram(m_iShaderPgmHandle);
	else
		std::cerr << "Error : Shader is nullptr - GLShader::Use" << std::endl;
}

void GLShader::UnUse()
{
	glUseProgram(0);
}
