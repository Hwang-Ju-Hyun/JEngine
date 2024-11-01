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