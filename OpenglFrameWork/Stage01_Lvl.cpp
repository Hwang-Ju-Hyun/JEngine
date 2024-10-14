#include "Stage01_Lvl.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "Sprite.h"
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
	int all_objs_size = Serializer::GetInstance()->GetObjectSize("json/GameObject/GameObject.json");	
	if (all_objs_size <= -1)
		return false;
	for (int i = 0; i < all_objs_size; i++)
	{
		m_vecTempObj.push_back(Serializer::GetInstance()->LoadGameObject("json/GameObject/GameObject.json", i + 1));		
	}
		
    return true;
}

bool Stage01_Lvl::Update()
{			
	return true;
}

bool Stage01_Lvl::Exit()
{
	GameObjectManager::GetInstance()->RemoveAllObjects();			
    return true;
}
