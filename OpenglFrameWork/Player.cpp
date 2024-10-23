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

void Player::SetDirection(glm::vec2 _dir)
{
	m_vDirection = _dir;
}

glm::vec2 Player::GetDirection() const
{
	return m_vDirection;
}

void Player::MoveMent()
{
    auto helper = GLHelper::GetInstance();
    glm::vec2 direction = { 0.f, 0.f };

    if (helper->GetUpArrowKeyPressed())
    {
        direction.y = 1.f; 
    }
    if (helper->GetDownArrowKeyPressed())
    {
        direction.y = -1.f;
    }
    if (helper->GetRightArrowKeyPressed())
    {
        direction.x = 1.f; 
    }
    if (helper->GetLeftArrowKeyPressed())
    {
        direction.x = -1.f; 
    }

    RigidBody* player_rig = (RigidBody*)m_pOwner->FindComponent("Rigid");
    if (player_rig)
    {
        if (direction != glm::vec2(0.f, 0.f)) 
        {
            player_rig->SetAccelation(direction * 10.f); 
        }
        else
        {
            player_rig->SetAccelation(glm::vec2(0.f, 0.f));
        }
    }
}

void Player::Update()
{
	MoveMent();
	
}

void Player::LoadFromJson(const json& _str)
{
}

json Player::SaveToJson(const json& _str)
{
	return json();
}
