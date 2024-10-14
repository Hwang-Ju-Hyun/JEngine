#include "ModelManager.h"
#include "GLModel.h"
#include "GLApp.h"
#include <iostream>
#include "header.h"

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
	//TODO : 모델들 가지고 있던거 다 삭제해줘야함
	for (int i = 0; i < GetAllModel().size(); i++)
	{
		delete m_vecModel[i];
		m_vecModel[i] = nullptr;
	}
}

std::vector<GLModel*> ModelManager::GetAllModel() const
{
	return m_vecModel;
}

GLModel* ModelManager::AddModel(GLModel* _model)
{	
	m_vecModel.push_back(_model);
	return _model;
}

GLModel* ModelManager::FindModel(const std::string& _ModelName)
{
	for (int i = 0; i < m_vecModel.size(); i++)
	{
		if (m_vecModel[i]->GetName() == _ModelName)
		{
			return m_vecModel[i];
		}
	}
	std::cerr << "Error : Can't find model - ModelManager::FindModel" << std::endl;
	return nullptr;
}

GLModel* ModelManager::FindModel(MODEL_TYPE _eModelType)
{
	for (int i = 0; i <= MODEL_TYPE::RECTANGLE; i++)
	{
		if (m_vecModel[i]->GetModelType() == _eModelType)
		{
			return m_vecModel[i];
		}
	}
}

bool ModelManager::Init()
{	
	if(!InitTriangle()||!InitRectangle())
		return false;	
	return true;
}

bool ModelManager::InitTriangle()
{
	std::string name = "Triangle";
	std::vector<glm::vec3> vertices =
	{
		glm::vec3{ -0.5f,-0.5f,0.0f},//Bottom Left
		glm::vec3{ -0.5f,0.5f,0.0f },//Top Left
		glm::vec3{ 0.5f,-0.5f,0.0f },//Bottom Right
		glm::vec3{0.5f,0.5f,0.0f}//Top Right
	};
	GLenum type = GL_TRIANGLES;	

	GLModel* model = new GLModel;
	model->CreateModel(type, vertices, name,MODEL_TYPE::TRIANGLE);
	if (model == nullptr)
	{
		std::cerr << "Error : model is nulltpr - ModelManager::InitTriangle" << std::endl;
		return false;
	}
	AddModel(model);		
	
	return true;
}

bool ModelManager::InitRectangle()
{
	const std::string& name = "Rectangle";
	std::vector<glm::vec3> vertices =
	{
		glm::vec3{ -0.5f,-0.5f,0.0f},//Bottom Left
		glm::vec3{ -0.5f,0.5f,0.0f },//Top Left
		glm::vec3{ 0.5f,-0.5f,0.0f },//Bottom Right
		glm::vec3{0.5f,0.5f,0.0f}//Top Right
	};
	std::vector<unsigned int> indices =
	{
		1,0,2,  //First Triangle
		1,2,3   //Second Triangle
	};

	GLenum type = GL_TRIANGLES;
	
	GLModel* model = new GLModel;
	model->CreateModel(type, vertices, name,MODEL_TYPE::RECTANGLE, GL_TRUE, indices);
	if (model == nullptr)
	{
		std::cerr << "Error : model is nulltpr - ModelManager::InitRectangle" << std::endl;
		return false;
	}
	AddModel(model);
	return true; 
}
