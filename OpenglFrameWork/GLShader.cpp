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
	//glCreateShader :  ���̴� ���α׷��� ������ ���̴��� �����ϸ� �� ID�� ��ȯ(�ڵ鰪)
	GLuint new_shader = glCreateShader(_shaderType); //���̴������� id��
	const GLchar* code[1];
	code[0] = _shaderCode;

	//glShaderSource : gpu�� �ν��� �� �ֵ��� �ҽ��ڵ带 �־��ִ� ����
	glShaderSource(new_shader, 1, code,NULL);

	GLint result = 0;
	GLchar err_log[1024] = { 0, };

	//�ڵ鰪�� ���� ���̴��� ������
	glCompileShader(new_shader);

	//���̴� ������ ���� Ȯ��
	glGetShaderiv(new_shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		//Shader�� Compile ���� Log Ȯ��
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

	//���̴� ���α׷��̶� ���� shader�� ������ ������ ����� �����Դϴ�.
	m_iShaderPgmHandle = glCreateProgram();
	if (m_iShaderPgmHandle<=0)
	{
		std::cout << "Error : Cannot create shader program. -GLShader::CompileShader" << std::endl;
		return false;
	}
	vs = AddShader(_vsCode, GL_VERTEX_SHADER);
	fs = AddShader(_fsCode, GL_FRAGMENT_SHADER);

	//glAttachShader() - Shader Program�� ����� Shader ���(Vertex, Fragment �ʼ�)
	glAttachShader(m_iShaderPgmHandle, vs);
	glAttachShader(m_iShaderPgmHandle, fs);

	GLint result = 0;
	GLchar err_log[1024] = { 0 };

	//���̴� ���ϵ��� ��ũ�ϰ� GPU���� ������ ���� ������ ����� OpenGL �Լ�.
	glLinkProgram(m_iShaderPgmHandle);
	//�ش� ����� ���α׷��� �ǰ� �������� ����������� Ȯ�� (��ŷ�� �ùٸ��� �Ǿ����� üũ)
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
