#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include "header.h"

class BaseComponent;
class GLModel;

class GameObject
{
public:
	GameObject()=delete;
	GameObject(const std::string& _name, const unsigned int _id);
	~GameObject();
private:
	GLModel* model = nullptr;
public:	
	void SetModelType(MODEL_TYPE _eModelType=MODEL_TYPE::TRIANGLE);
	GLModel* GetModel()const;
	MODEL_TYPE GetModelType()const;
private:	
	std::string m_strName="Object Nothing";
	unsigned int m_iID = 0;
private:
	GLuint m_iModel_ref = 0;
	GLuint m_iShader_ref=0;
public:
	void SetShaderRef(GLuint _ref);
	GLuint GetShaderRef()const;
	void SetModelRef(GLuint _ref);
	GLuint GetModelRef()const;
public:
	void SetID(unsigned int _id);
	unsigned int GetID()const;
	void SetName(const std::string& _name);
	std::string GetName()const;
public:
	void Init();
	void Update(GLdouble _dt);
	void Draw();
public:
	std::vector<std::pair<std::string, BaseComponent*>> m_vecComponentOfObj;
public:
	BaseComponent* AddComponent(const std::string _compName, BaseComponent* _comp);
	BaseComponent* FindComponent(const std::string& _name);
	std::vector<std::pair<std::string, BaseComponent*>> GetAllComponentOfObj()const;
	void DeleteComponent(const std::string& _name);
public:
	friend class BaseComponent;
};