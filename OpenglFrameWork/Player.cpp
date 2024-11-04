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
#include "TileEditor.h"
#include "Prefabs.h"
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

void Player::SetHp(int _hp)
{
    m_iHP = _hp;
}

int Player::GetHp() const
{
    return m_iHP;
}

void Player::SetMaxBombCnt(int _cnt)
{
    m_iMaxBombCnt = _cnt;
}

void Player::AddMaxBombCnt(int _cnt)
{
    m_iMaxBombCnt += _cnt;
}

int Player::GetMaxBombCnt() const
{
    return m_iMaxBombCnt;
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

    Transform* player_trs = static_cast<Transform*>((Transform*)m_pOwner->FindComponent(Transform::TransformTypeName));
    glm::mat3 mat = {player_trs->GetScreenByWorld()};
    glm::vec2 pos = { mat[2][0],mat[2][1] };
    glm::vec2 grid=TileEditor::GetInstance()->GetScreenGridByScreenPoint(pos);
    player_trs->SetGridByScreenPos(grid);
    grid = player_trs->GetGridByScreenPos();
    std::cout <<grid.x << "," << grid.y << std::endl;
}   
    
void Player::Attack()
{   
    auto Helper = GLHelper::GetInstance();    
    bool init = false;
    if (Helper->GetSpaceKeyPressed())
    {          
        if (m_iCurBombCnt <= m_iMaxBombCnt)
        {
            Bomb* bomb_comp= Prefabs::GetInstance()->CreateBombs("json/Bomb/Bomb.json", this->GetOwner());            
            m_iCurBombCnt++;
        }
        
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

        auto bomb_cnt = comp_data->find("MaxBomb_Cnt");
        m_iMaxBombCnt = bomb_cnt->begin().value();
    }
}

json Player::SaveToJson(const json& _str)
{
    json data;

    data["Type"] = "Player";

    json comp_data;
    comp_data["Current Level"] = GetCurrentLevel()->GetName();
    comp_data["HP"] = m_iHP;    
    comp_data["MaxBomb_Cnt"] = m_iMaxBombCnt;

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