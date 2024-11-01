#include "Wall.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>

Wall::Wall(GameObject* _owner)
	:BaseComponent(_owner)
{

}

Wall::~Wall()
{

}

void Wall::SetScreeGrid(glm::vec2 _grid)
{
	m_vScreenGrid = _grid;
}

glm::vec2 Wall::GetScreenGrid() const
{
	return m_vScreenGrid;
}

void Wall::SetExist(bool _exist)
{
	m_bExist = _exist;
}

bool Wall::GetExist() const
{
	return m_bExist;
}

void Wall::SetFragile(bool _fragile)
{
	m_bFragile = _fragile;
}

bool Wall::GetFragile() const
{
	return m_bFragile;
}

void Wall::LoadFromJson(const json& _str)
{	
	auto comp_data = _str.find("CompData");
	if (comp_data != _str.end())
	{
		auto grid = comp_data->find("Grid");
		m_vScreenGrid.x = grid->begin().value();
		m_vScreenGrid.y = (grid->begin()+1).value();
		
		auto exist = comp_data->find("Exist");
		m_bExist = exist->begin().value();

		auto fragile = comp_data->find("Fragile");
		m_bFragile = fragile->begin().value();
	}
}

json Wall::SaveToJson(const json& _str)
{
	json data;

	data["Type"] = "Wall";

	json comp_data;
	comp_data["Grid"] = { m_vScreenGrid.x,m_vScreenGrid.y };
	comp_data["Exist"] = m_bExist;
	comp_data["Fragile"] = m_bFragile;

	data["CompData"] = comp_data;

	return data;
}

BaseRTTI* Wall::CreateWallComponent()
{
	GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
	BaseRTTI* p = last_obj->AddComponent(Wall::WallTypeName, new Wall(last_obj));
	if (p == nullptr)
	{
		std::cerr << "Error : BaseRTTI is nullptr - Player::CreatePlayerComponent" << std::endl;
		return nullptr;
	}
	return p;
}
