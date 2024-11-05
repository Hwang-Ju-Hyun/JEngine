#pragma once
#include "BaseComponent.h"
#include <glm.hpp>

class Collision;

class Wall:
	public BaseComponent
{
public:
	Wall(GameObject* _owner);
	virtual ~Wall()override;
private:
	glm::vec2 m_vScreenGrid = {};
	bool m_bExist = false;
	bool m_bFragile = false;
private:
	Collision* m_pCol = nullptr;
public:
	void SetScreeGrid(glm::vec2 _grid);
	glm::vec2 GetScreenGrid()const;
	void SetExist(bool _exist);
	bool GetExist()const;
	void SetFragile(bool _fragile);
	bool GetFragile()const;
public:
	virtual void EventCollision(Collision* _pOther)override;
public:
	virtual void LoadFromJson(const json& _str);
	virtual json SaveToJson(const json& _str);
public:
	static constexpr const char* WallTypeName = "Wall";
	static BaseRTTI* CreateWallComponent();
};