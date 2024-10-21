#include "GLModel.h"
#include "ModelManager.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLModel::GLModel()
{	
}

GLModel::~GLModel()
{

}

void GLModel::SetName(const std::string& _ModelName)
{
	m_strName = _ModelName;
}

std::string GLModel::GetName() const
{
	return m_strName;
}

void GLModel::SetModelType(MODEL_TYPE _eModelType)
{
	m_eModelType = _eModelType;
}

MODEL_TYPE GLModel::GetModelType() const
{
	return m_eModelType;
}

void GLModel::SetVerticesCnt(int _count)
{
	m_iVerticesCnt = _count;
}

int GLModel::GetVerticesCnt() const
{
	return m_iVerticesCnt;
}

void GLModel::SetIndicesCnt(int _count)
{
	m_iIndicesCnt = _count;
}

int GLModel::GetIndicesCnt() const
{
	return m_iIndicesCnt;
}

void GLModel::SetVertices(const std::vector<glm::vec3> _vertices)
{
	for (int i = 0; i < _vertices.size(); i++)
		m_vVertices.push_back(_vertices[i]);
}

std::vector<glm::vec3> GLModel::GetVertices() const
{
	return m_vVertices;
}

void GLModel::SetIndices(std::vector<unsigned int> _indices)
{
	m_vIndices = _indices;
}

std::vector<unsigned int> GLModel::GetIndices() const
{
	return m_vIndices;
}

void GLModel::SetPrimitiveType(GLenum _PrimitiveType)
{
	m_ePrimitive_type = _PrimitiveType;
}

GLenum GLModel::GetPrimitiveType() const
{
	return m_ePrimitive_type;
}

void GLModel::SetVAO(GLuint _vao)
{
	m_iVAO = _vao;
}

void GLModel::SetVBO(GLuint _vbo)
{
	m_iVBO = _vbo;
}

void GLModel::SetEBO(GLuint _ebo)
{
	m_iEBO = _ebo;
}

GLuint GLModel::GetVAO() const
{
	return m_iVAO;
}

GLuint GLModel::GetVBO() const
{
	return m_iVBO;
}

GLuint GLModel::GetEBO() const
{
	return m_iEBO;
}

void GLModel::CreateModel(GLenum _PrimitveType, std::vector<glm::vec3> _vertices, const std::string& _name, enum MODEL_TYPE _eModelType,GLboolean _ebo, std::vector<unsigned int>_indices)
{
	SetName(_name);	

	SetVertices(_vertices);

	SetModelType(_eModelType);

	auto VAO = GetVAO();
	//1���� ����ڴ�
	glGenVertexArrays(1, &VAO);
	//���� �ֱ� vao�� Opengl context�� ���ε�
	glBindVertexArray(VAO);
	if (VAO <= 0)
	{
		std::cerr << "Error : VAO Fail - ModelManager::InitTriangle" << std::endl;
		return;
	}
	//VBO�� ����ڴ� 
	//VBO�� 1�� ����ڴ�	
	auto VBO = GetVBO();
	glGenBuffers(1, &VBO);
	//GL_ARRAY_BUFFER��� ���ĵ忡,VBO��� å(�Ǵ� �ƹ��ų�)�� ���ε�(����)   
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	if (VBO <= 0)
	{
		std::cerr << "Error : VBO Fail - ModelManager::InitTriangle" << std::endl;
		return ;
	}
	
	// glBufferData�� ���� å(������) � ������ ������ �ִ��� ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

	//���� �Լ������� 0��° location�� vao�� 0��° attribute�� ����!
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);

	glEnableVertexAttribArray(0);	











	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// �ؽ�ó wrapping/filtering �ɼ� ����(���� ���ε��� �ؽ�ó ��ü�� ����)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// �ؽ�ó �ε� �� ����
	int width, height, nrChannels;
	unsigned char* data = stbi_load("../Extern/Assets/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glBindTexture(GL_TEXTURE_2D, texture);
	









	
	if (_ebo)
	{
		auto EBO = GetEBO();
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		if (EBO <= 0)
		{
			std::cerr << "Error : EBO Fail - ModelManager::InitRectangle" << std::endl;
			return;
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
		SetEBO(EBO);
		
		SetIndicesCnt(_indices.size());
	}

	SetPrimitiveType(_PrimitveType);
	SetVAO(VAO);
	SetVBO(VBO);
	SetVerticesCnt(_vertices.size());

	

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	











	


	return;	
}

void GLModel::Draw()
{
	glBindVertexArray(GetVAO());
	if (GetEBO() <= 0)	
		glDrawArrays(GetPrimitiveType(), 0, GetVerticesCnt());	
	else
		glDrawElements(GetPrimitiveType(), GetIndicesCnt(), GL_UNSIGNED_INT, 0);
}

GLModel* GLModel::GetModelFromImgui()
{	
	const char* TreeName = "Model";
	auto models=ModelManager::GetInstance()->GetAllModel();

	if (ImGui::TreeNode(TreeName))
	{
		for (int i = 0; i < models.size(); i++)
		{
			if (ImGui::Button(models[i]->GetName().c_str()))
			{
				ImGui::TreePop();
				return models[i];
			}
		}
		ImGui::TreePop();		
	}
	return nullptr;
}
