#include "Stage01_Lvl.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "Transform.h"
#include "GLModel.h"
#include "Player.h"
#include "RigidBody.h"
#include "Serializer.h"
#include "ResourceManager.h"
	
Stage01_Lvl::Stage01_Lvl(std::string _name)	
	:BaseLevel(_name)
{	
}	
	
Stage01_Lvl::~Stage01_Lvl()
{
}

bool Stage01_Lvl::Init()
{				
	Serializer::GetInstance()->LoadStage("json/" + GetName()+"/"+ GetName() + ".txt");

	{
		GameObject* bomb_obj = new GameObject("bomb", 0);
		Transform* bomb_trs = static_cast<Transform*>(bomb_obj->AddComponent(Transform::TransformTypeName, new Transform(bomb_obj)));
		Sprite* bomb_spr = static_cast<Sprite*>(bomb_obj->AddComponent(Sprite::SpriteTypeName, new Sprite(bomb_obj)));
		bomb_trs->SetPosition({ -900.f,-300.f });
		bomb_trs->SetScale({ 100.f,100.f });
		bomb_obj->SetModelType(MODEL_TYPE::CIRCLE);
	}
	
	//Serializer::GetInstance()->SaveStage("json/" + GetName() + "/" + GetName() + ".txt");
	
    return true;
}	
		
bool Stage01_Lvl::Update()
{			
	return true;
}	
	
bool Stage01_Lvl::Exit()
{	
	ResourceManager::GetInstance()->RemoveAllRes();
	GameObjectManager::GetInstance()->RemoveAllObjects();			
    return true;
}

	