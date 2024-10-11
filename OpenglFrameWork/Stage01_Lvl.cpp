#include "Stage01_Lvl.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "ModelManager.h"
#include "Transform.h"
#include "GLModel.h"
#include "Serializer.h"


Stage01_Lvl::Stage01_Lvl()
{
}

Stage01_Lvl::~Stage01_Lvl()
{
}

bool Stage01_Lvl::Init()
{
	temp_obj1 = Serializer::GetInstance()->LoadGameObject("json/GameObject/GameObject.json");	
	Transform* temp_trs1 = static_cast<Transform*>((temp_obj1->FindComponent("Transform")));
	GLModel* temp_tri1 = ModelManager::GetInstance()->FindModel("Triangle");	
	temp_obj1->SetModel(temp_tri1);
	temp_trs1->SetScale({ 0.5f,0.5f });	

	temp_obj2 = Serializer::GetInstance()->LoadGameObject("json/GameObject/GameObject.json");
	Transform* temp_trs2 = static_cast<Transform*>((temp_obj2->FindComponent("Transform")));
	temp_trs2->SetScale({ 0.5f,0.5f });	
	GLModel* temp_tri2 = ModelManager::GetInstance()->FindModel("Rectangle");
	temp_obj2->SetModel(temp_tri2);	

    return true;
}

bool Stage01_Lvl::Update()
{
	Transform* temp_trs1 = static_cast<Transform*>(temp_obj1->FindComponent("Transform"));
	Transform* temp_trs2 = static_cast<Transform*>(temp_obj2->FindComponent("Transform"));
	
	temp_trs1->AddPositionX(0.003f);	
	temp_trs2->AddPositionX(-0.003f);			
	return true;
}

bool Stage01_Lvl::Exit()
{
	GameObjectManager::GetInstance()->RemoveAllObjects();
	auto a=GameObjectManager::GetInstance()->GetAllObject();
	a.capacity();
	auto b = ComponentManager::GetInstance()->GetAllComponents();	
	b.capacity();
	int c = 0;
    return true;
}
