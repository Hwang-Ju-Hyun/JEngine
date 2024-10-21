#pragma once
#include "single.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include "header.h"


class GLModel //Mesh
{
public:
	GLModel();
	virtual ~GLModel();
private:
	std::string m_strName="Nothing";
public:
	void SetName(const std::string& _ModelName);
	std::string GetName()const;
private:

private:
	GLenum m_ePrimitive_type=-1;
	GLuint m_iVAO = 0;	
	GLuint m_iVBO = 0;
	GLuint m_iEBO = 0;
	GLuint m_iShader = 0;
	int m_iVerticesCnt = -1;
	int m_iIndicesCnt = -1;
	enum MODEL_TYPE m_eModelType = TRIANGLE;
private:
	std::vector<glm::vec3> m_vVertices;
	std::vector<unsigned int> m_vIndices;

public:
	void SetModelType(enum MODEL_TYPE _eModelType);
	MODEL_TYPE GetModelType()const;
public:	
	void SetVerticesCnt(int _count);	
	int  GetVerticesCnt()const;
	void SetIndicesCnt(int _count);
	int GetIndicesCnt()const;
public:
	void SetVertices(const std::vector<glm::vec3> _vertices);
	std::vector<glm::vec3>GetVertices()const;
	void SetIndices(std::vector<unsigned int> _indices);
	std::vector<unsigned int>GetIndices()const;
public:
	void SetPrimitiveType(GLenum _PrimitiveType);
	GLenum GetPrimitiveType()const;
public:
	void SetVAO(GLuint _vao);
	void SetVBO(GLuint _vbo);
	void SetEBO(GLuint _ebo);
	GLuint GetVAO()const;	
	GLuint GetVBO()const;
	GLuint GetEBO()const;
public:
	void CreateModel(GLenum _PrimitveType, std::vector<glm::vec3> _vertices, const std::string& _name,enum MODEL_TYPE _eModelType,GLboolean _ebo = GL_FALSE, std::vector<unsigned int>_indices = {});
public:
	void Draw();
public:	
	GLModel* GetModelFromImgui();
};

