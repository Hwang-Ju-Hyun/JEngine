#include "Player.h"
#include "GLHelper.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameObject.h"
#include "RigidBody.h"

Player::Player(GameObject* _owner)
	:BaseComponent(_owner)
{
}

Player::~Player()
{
}

void Player::MoveMent()
{
	auto helper = GLHelper::GetInstance();
	Transform* player_trs = (Transform*)m_pOwner->FindComponent("Transform");
	RigidBody* player_rig = (RigidBody*)m_pOwner->FindComponent("Rigid");

	//I will add the delta time in the future but not NOW
	if (helper->GetUpArrowKeyPressed())
	{
		player_rig->
	}
	if (helper->GetDownArrowKeyPressed())
	{
		player_trs->AddPositionY(-10.f);
	}
	if (helper->GetRightArrowKeyPressed())
	{
		player_trs->AddPositionX(10.f);
	}
	if (helper->GetLeftArrowKeyPressed())
	{
		player_trs->AddPositionX(-10.f);
	}
}

void Player::Update()
{
	MoveMent();
	
}
