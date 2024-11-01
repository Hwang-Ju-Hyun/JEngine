#include "Player.h"
#include "GLHelper.h"
#include "Transform.h"
#include "BaseLevel.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "RigidBody.h"
#include "BaseLevel.h"
#include "GameObjectManager.h"
#include "Bomb.h"
#include "Serializer.h"
#include "Sprite.h"
#include <iostream>

Player::Player(GameObject* _owner)
	:BaseComponent(_owner)
{
    m_pPlayerTrs = static_cast<Transform*>(_owner->FindComponent(Transform::TransformTypeName));
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

void Player::SetCurrentLevel(BaseLevel* _level)
{
    m_pCurrentLevel = _level;
}

BaseLevel* Player::GetCurrentLevel() const
{
    return GameStateManager::GetInstance()->GetCurrentLevel();
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

    RigidBody* player_rig = (RigidBody*)m_pOwner->FindComponent(RigidBody::RigidBodyTypeName);    
    if (player_rig)
    {
        if (direction != glm::vec2{0.f, 0.f})
        {
            player_rig->SetAccelation(direction * 3000.f);             
        }
        else
        {
            player_rig->SetAccelation(glm::vec2{ 0.f, 0.f });
            player_rig->SetVelocity(glm::vec2{0.f,0.f});
        }
    }    
}
    
void Player::Attack()
{   
    auto Helper = GLHelper::GetInstance();    
    if (Helper->GetSpaceKeyPressed())
    {   
        //Prefabs
        GameObject* bomb_obj = Serializer::GetInstance()->LoadJson("json/Bomb/Bomb.json",true);
        Transform* bomb_trs = static_cast<Transform*>(bomb_obj->FindComponent(Transform::TransformTypeName));
        Sprite* bomb_spr = static_cast<Sprite*>(bomb_obj->FindComponent(Sprite::SpriteTypeName));
        bomb_trs->SetPosition(m_pPlayerTrs->GetPosition());
        bomb_trs->SetScale({ 50.f,50.f });
        bomb_obj->SetModelType(MODEL_TYPE::CIRCLE);
    }
}

void Player::Update()
{
	MoveMent();	
    Attack();
}

void Player::LoadFromJson(const json& _str)
{
    auto comp_data = _str.find("CompData");
    if (comp_data != _str.end())
    {
        auto cur_lvl = comp_data->find("Current Level");
        m_sCurrentLevelName = cur_lvl->begin().value();

        auto hp = comp_data->find("HP");
        m_iHP = hp->begin().value();
    }
}

json Player::SaveToJson(const json& _str)
{
    json data;

    data["Type"] = "Player";

    json comp_data;
    comp_data["Current Level"] = GetCurrentLevel()->GetName();
    comp_data["HP"] = m_iHP;

    data["CompData"] = comp_data;

    return data;
}

BaseRTTI* Player::CreatePlayerComponent()
{
    GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
    BaseRTTI* p = last_obj->AddComponent(Player::PlayerTypeName, new Player(last_obj));
    if (p == nullptr)
    {
        std::cerr << "Error : BaseRTTI is nullptr - Player::CreatePlayerComponent" << std::endl;
        return nullptr;
    }
    return p;
}

void Player::SpawBawn()
{
    /*m_pBomb = new GameObject("Bomb", 0);
    Bomb* obj_bomb=static_cast<Bomb*>(m_pBomb->AddComponent("Bomb", new Bomb(m_pBomb)2));
    obj_bomb->SetBombMaster(m_pBomb);
    obj_bomb->SpawnBomb(m_pBomb);*/
}