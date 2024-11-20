#include "Enemy.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>


Enemy::Enemy(GameObject* _owner)
	:BaseComponent(_owner)
{
}

Enemy::~Enemy()
{
}

void Enemy::SetHp(int _hp)
{
	m_iHP = _hp;
}

int Enemy::GetHp() const
{
	return m_iHP;
}

void Enemy::SetMaxBombCnt(int _cnt)
{
	m_iMaxBombCnt = _cnt;
}

void Enemy::AddMaxBombCnt(int _cnt)
{
	m_iMaxBombCnt += _cnt;
}

int Enemy::GetMaxBombCnt() const
{
	return m_iMaxBombCnt;
}

void Enemy::Update()
{
}

void Enemy::EventCollision(Collision* _pOther)
{
}

void Enemy::LoadFromJson(const json& _str)
{
    auto comp_data = _str.find("CompData");
    if (comp_data != _str.end())
    {        
        auto hp = comp_data->find("HP");
        m_iHP = hp->begin().value();        

        auto bomb_cnt = comp_data->find("MaxBomb_Cnt");
        m_iMaxBombCnt = bomb_cnt->begin().value();        
    }
}

json Enemy::SaveToJson(const json& _str)
{
    json data;

    data["Type"] = "Enemy";

    json comp_data;    
    comp_data["HP"] = m_iHP;
    comp_data["MaxBomb_Cnt"] = m_iMaxBombCnt;

    data["CompData"] = comp_data;

    return data;
}

BaseRTTI* Enemy::CreateEnemyComponent()
{
    GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
    BaseRTTI* p = last_obj->AddComponent(Enemy::EnemyTypeName, new Enemy(last_obj));
    if (p == nullptr)
    {
        std::cerr << "Error : BaseRTTI is nullptr - Player::CreatePlayerComponent" << std::endl;
        return nullptr;
    }
    return p;
}
