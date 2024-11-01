#include "ModelManager.h"
#include "GLModel.h"
#include "GLApp.h"
#include <iostream>
#include "MathManager.h"
#include "header.h"
#include "TileEditor.h"

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
	for (int i = 0; i <= MODEL_TYPE::GRID_LINE; i++)
	{
		if (m_vecModel[i]->GetModelType() == _eModelType)
		{
			return m_vecModel[i];
		}
	}
}

bool ModelManager::Init()
{	
	if(!InitTriangle()||!InitRectangle()||!InitCircle()||!InitGrid())
		return false;	
	return true;
}

bool ModelManager::InitGrid()
{
	std::string name = "GridLine";	
	std::vector<glm::vec3> vertices;
		
	int stacks = TileEditor::GetInstance()->GetNumberOfWallHeight();
	int slices = TileEditor::GetInstance()->GetNumberOfWallWidth();
	
	GLfloat const u { 2.f / static_cast<GLfloat>(slices) };
	GLfloat const v { 2.f / static_cast<GLfloat>(stacks) };

	for (GLint col=0; col <= slices; ++col)
	{
		GLfloat x = (u * static_cast<GLfloat>(col)) - 1.f;
		vertices.emplace_back(glm::vec2(x, -1.f),0.f); 
		vertices.push_back(glm::vec3{ 0.f,0.f,0.f });  //texture
		vertices.emplace_back(glm::vec2(x, 1.f),0.f);  
		vertices.push_back(glm::vec3{ 0.f,0.f,0.f });  //texture
	}
	for (GLint row=0; row <= stacks; ++row)
	{
		GLfloat y =(v * static_cast<GLfloat>(row)) - 1.f;
		vertices.emplace_back(glm::vec2(-1.f, y), 0.f);
		vertices.push_back(glm::vec3{ 0.f,0.f,0.f });  //texture
		vertices.emplace_back(glm::vec2(1.f, y), 0.f);
		vertices.push_back(glm::vec3{ 0.f,0.f,0.f });  //texture
	}
	
	GLenum type = GL_LINES;
	
	GLModel* model = new GLModel;

	model->CreateModel(type, vertices, name, MODEL_TYPE::GRID_LINE);
	
	if (model == nullptr)
	{
		std::cerr << "Error : model is nulltpr - ModelManager::InitGrid_Line" << std::endl;
		return false;
	}		
	AddModel(model);
}

bool ModelManager::InitCircle()
{
	std::string name = "Circle";
	int slice = 300;
	std::vector<glm::vec3> vertices;
	vertices.push_back({ glm::vec3{0.f,0.f,0.f} });//pivot
	float angle = (2.0f * 3.141592f) / slice;
	for (int i = 1; i < slice + 3; i++)
	{
		vertices.push_back({ glm::vec2{ cos(angle * (i - 1))/2.f,sin(angle * (i - 1)) / 2.f},0.f });
	}
	GLenum type = GL_TRIANGLE_FAN;

	GLModel* model = new GLModel;
	model->CreateModel(type, vertices, name, MODEL_TYPE::CIRCLE);
	if (model == nullptr)
	{
		std::cerr << "Error : model is nulltpr - ModelManager::InitCircle" << std::endl;
		return false;
	}
	AddModel(model);

	return true;
}

//TODO : 나중에 정점도 입력받아서 봐꿀 수 있게 끔 해주자
bool ModelManager::InitTriangle()
{
	std::string name = "Triangle";
	std::vector<glm::vec3> vertices =
	{
		glm::vec3{ -0.5f,-0.5f,0.0f},//Bottom Left
		glm::vec3{0.f,0.f,0.f},//Bottom Left - textrue


		glm::vec3{ -0.5f,0.5f,0.0f },//Top Left
		glm::vec3{0.f,1.f,0.f},//Top Left - texture

		glm::vec3{ 0.5f,-0.5f,0.0f },//Bottom Right
		glm::vec3{1.f,0.f,0.f},//Bottom Right - texture
	};	
	
	std::vector<glm::vec3> edges;	
	
	edges.push_back(glm::vec3{1.f,0.f,0.f});
	edges.push_back(glm::vec3{-1.f,1.f,0.f});
	edges.push_back(glm::vec3{ 1.f,0.f,0.f });

	GLenum type = GL_TRIANGLES;	

	GLModel* model = new GLModel;	
	model->SetEdges(edges);
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
		glm::vec3{0.f,0.f,0.f},//Bottom Left - textrue


		glm::vec3{ -0.5f,0.5f,0.0f },//Top Left
		glm::vec3{0.f,1.f,0.f},//Top Left - texture

		glm::vec3{ 0.5f,-0.5f,0.0f },//Bottom Right
		glm::vec3{1.f,0.f,0.f},//Bottom Right - texture

		glm::vec3{0.5f,0.5f,0.0f},//Top Right
		glm::vec3{1.f,1.f,0.f} //Top Right - texture
	};	

	std::vector<unsigned int> indices =
	{
		1,0,2,  //First Triangle
		1,2,3   //Second Triangle
	};

	std::vector<glm::vec3> edges;	
	edges.push_back(glm::vec3{ 1.f,0.f,0.f });//bottom
	edges.push_back(glm::vec3{ 0.f,1.f,0.f });//right
	edges.push_back(glm::vec3{ -1.f,0.f,0.f });//top
	edges.push_back(glm::vec3{ 0.f,-1.f,0.f });//left

	GLenum type = GL_TRIANGLES;
	
	GLModel* model = new GLModel;
	model->SetEdges(edges);
	model->CreateModel(type, vertices, name,MODEL_TYPE::RECTANGLE, GL_TRUE, indices);
	if (model == nullptr)
	{
		std::cerr << "Error : model is nulltpr - ModelManager::InitRectangle" << std::endl;
		return false;
	}
	AddModel(model);
	return true; 
}
