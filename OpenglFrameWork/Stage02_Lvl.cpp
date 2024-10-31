#include "Stage02_Lvl.h"
#include "Serializer.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"

Stage02_Lvl::Stage02_Lvl(const std::string _name)
	:BaseLevel(_name)
{
}

Stage02_Lvl::~Stage02_Lvl()
{
}

bool Stage02_Lvl::Init()
{		
	Serializer::GetInstance()->LoadStage("json/" + GetName() + "/" + GetName() + ".txt");
	CollisionManager::GetInstance()->Init();//Todo: �ӽ��ڵ� �� ���� �� ����
	return true;
}

bool Stage02_Lvl::Update()
{
	return false;
}

bool Stage02_Lvl::Exit()
{
	ResourceManager::GetInstance()->RemoveAllRes();
	GameObjectManager::GetInstance()->RemoveAllObjects();
	return true;
}
