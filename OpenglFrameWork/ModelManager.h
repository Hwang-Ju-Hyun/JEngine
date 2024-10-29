#pragma once
#include "single.h"
#include "GLModel.h"
#include <map>

class GLModel;

class ModelManager
{
public:
	SINGLE(ModelManager);
private:	
	std::vector<GLModel*> m_vecModel;
public:				
	std::vector<GLModel*> GetAllModel()const;//»ó¹«´Ô <- À¯³äÇØ¼­ ¾²±â
	GLModel* AddModel(GLModel* _model);
	GLModel* FindModel(const std::string& _ModelName);
	GLModel* FindModel(MODEL_TYPE _eModelType);
public:
	bool Init();
public:
	bool InitGrid();
	bool InitCircle();
	bool InitTriangle();
	bool InitRectangle();		
};