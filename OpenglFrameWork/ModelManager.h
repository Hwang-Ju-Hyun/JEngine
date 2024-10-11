#pragma once
#include "header.h"
#include <map>
#include "GLModel.h"

class GLModel;

class ModelManager
{
public:
	SINGLE(ModelManager);
private:	
	std::vector<GLModel*> m_vecModel;
public:				
	std::vector<GLModel*> GetAllModel()const;//�󹫴� <- �����ؼ� ����
	GLModel* AddModel(GLModel* _model);
	GLModel* FindModel(const std::string& _ModelName);
public:
	bool Init();	
public:
	bool InitTriangle();
	bool InitRectangle();
};