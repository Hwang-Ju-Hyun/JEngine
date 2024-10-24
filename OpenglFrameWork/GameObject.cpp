#include "GameObject.h"
#include "GameObjectManager.h"
#include "ShaderManager.h"
#include "GLShader.h"
#include "ComponentManager.h"
#include "BaseComponent.h"
#include "ModelManager.h"
#include "GLModel.h"
#include "TextureResource.h"
#include "Sprite.h"
#include "GLApp.h"
#include <iostream>


GameObject::GameObject(const std::string& _name,const unsigned int _id)	
	:m_strName(_name),
	m_iID(_id)
{
	GameObjectManager::GetInstance()->AddObject(this);	
}

GameObject::~GameObject()
{
	for (int i = 0; i < m_vecComponentOfObj.size(); i++)
	{
		DeleteComponent(m_vecComponentOfObj[i].first);
	}
}

void GameObject::SetModelType(MODEL_TYPE _eModelType)
{
	model=ModelManager::GetInstance()->FindModel(_eModelType);
}

GLModel* GameObject::GetModel() const
{
	return model;
}

MODEL_TYPE GameObject::GetModelType() const
{
	if (model == nullptr)
		std::cerr << "Error : model is nullptr - GameObject::GetModelType" << std::endl;
	else
		return model->GetModelType();
}

void GameObject::SetShaderRef(GLuint _ref)
{
	m_iShader_ref = _ref;
}

GLuint GameObject::GetShaderRef() const
{
	return m_iShader_ref;
}

void GameObject::SetModelRef(GLuint _ref)
{
	m_iModel_ref = _ref;
}

GLuint GameObject::GetModelRef() const
{
	return m_iModel_ref;
}

void GameObject::SetID(unsigned int _id)
{
	m_iID = _id;
}

unsigned int GameObject::GetID() const
{
	return m_iID;
}

void GameObject::SetName(const std::string& _name)
{
	m_strName = _name;
}

std::string GameObject::GetName() const
{
	return m_strName;
}

void GameObject::Init()
{
	m_iModel_ref  = 0;
	m_iShader_ref = 0;
}

void GameObject::Update(GLdouble _dt)
{
}

void GameObject::Draw()
{	
	if (model)
		model->Draw();	
}

BaseComponent* GameObject::AddComponent(const std::string _compName, BaseComponent* _comp)
{
	_comp->m_pOwner = this;
	m_vecComponentOfObj.push_back({ _compName,_comp });
	auto comp=ComponentManager::GetInstance()->AddComponent(_compName, _comp);
	return comp;
}

BaseComponent* GameObject::FindComponent(const std::string& _name)
{
	for (int i = 0; i < m_vecComponentOfObj.size(); i++)
	{
		if (m_vecComponentOfObj[i].first == _name)
			return m_vecComponentOfObj[i].second;
	}	
	return nullptr;	
}

std::vector<std::pair<std::string, BaseComponent*>> GameObject::GetAllComponentOfObj() const
{
	return m_vecComponentOfObj;
}

void GameObject::DeleteComponent(const std::string& _name)
{
	//벡터의 assignment 연산자는 깊은 복사를 하므로 이렇게 할당해줘도 아랫 코드가 정상적으로 작동한다
	//굳이 왜 할당했는가 : m_vecComponentOfObj <- 넘 길다 손가락 아픔

	auto comps = m_vecComponentOfObj;

	for (int i = 0; i < comps.size(); i++)
	{
		if (comps[i].first == _name)
		{
			ComponentManager::GetInstance()->RemoveComponent(comps[i].second);
			delete comps[i].second;
			comps.erase(comps.begin()+i);
		}
	}	
}

void GameObject::SetTexture(TextureResource* _res)
{
	Sprite* spr = (Sprite*)FindComponent("Sprite");
	if (spr != nullptr)
	{
		spr->SetTexture(_res);
	}
	else
		spr->SetTexture(nullptr);
}

TextureResource* GameObject::GetTexture()
{
	Sprite* spr=(Sprite*)FindComponent("Sprite");	
	TextureResource* texture = spr->GetTexture();
	if (texture != nullptr)
		return 	texture;
	return nullptr;		
}
