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
	const std::string go_path = "json/" + GetName() + "/GameObject.json";
	const std::string wall_path = "json/" + GetName() + "/Wall.json";		
	const std::string player_path = "json/Player/Player.json";

	int all_objs_size = Serializer::GetInstance()->GetObjectSize(go_path);
	int all_walls_size = Serializer::GetInstance()->GetObjectSize(wall_path);
	int all_player_size = Serializer::GetInstance()->GetObjectSize(player_path);
	if (all_objs_size <= -1|| all_walls_size<=-1)
		return false;
	
	for (int i = 0; i < all_objs_size; i++)	
		m_vecGameObject_Lvl01.push_back(Serializer::GetInstance()->LoadGameObject(go_path, i + 1));	
	for (int i = 0; i < all_walls_size; i++)
		m_vecWall_Lv01.push_back(Serializer::GetInstance()->LoadWall(wall_path, i + 1));	

	if(all_player_size>=0)
		m_pPlayer=Serializer::GetInstance()->LoadPlayer(player_path, 1);

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
