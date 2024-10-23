#include "Stage02_Lvl.h"
#include "Serializer.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"

Stage02_Lvl::Stage02_Lvl(const std::string _name)
	:BaseLevel(_name)
{
}

Stage02_Lvl::~Stage02_Lvl()
{
}

bool Stage02_Lvl::Init()
{
	const std::string go_path = "json/" + GetName() + "/GameObject.json";
	const std::string wall_path = "json/" + GetName() + "/Wall.json";

	int all_objs_size = Serializer::GetInstance()->GetObjectSize(go_path);
	int all_walls_size = Serializer::GetInstance()->GetObjectSize(wall_path);
	if (all_objs_size <= -1 || all_walls_size <= -1)
		return false;
	for (int i = 0; i < all_objs_size; i++)
		m_vecGameObject_Lvl02.push_back(Serializer::GetInstance()->LoadGameObject(go_path, i + 1));
	for (int i = 0; i < all_walls_size; i++)
		m_vecWall_Lv02.push_back(Serializer::GetInstance()->LoadWall(wall_path, i + 1));

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
