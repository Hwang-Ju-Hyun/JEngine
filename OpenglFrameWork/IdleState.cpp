#include "IdleState.h"
#include <random>
#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include "GameObjectManager.h"
#include "Enemy.h"
#include "TimeManager.h"
#include "GLHelper.h"

IdleState::IdleState()
{
    m_pEnemy = GameObjectManager::GetInstance()->FindObject(Enemy::EnemyTypeName);
}

IdleState::~IdleState()
{
}

void IdleState::Init()
{
   
}

void IdleState::Update()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rand_dist(0, 3);    
    int rand_dir = rand_dist(mt);    
    int range = rand_dist(mt);

    Transform* enemy_trs = static_cast<Transform*>(m_pEnemy->FindComponent(Transform::TransformTypeName));
    glm::vec2 cur_enemy_grid=enemy_trs->GetGridByScreenPos();    
    
    static int nextX=0;
    static int nextY=0;

    float dt = TimeManager::GetInstance()->GetDeltaTime();
    static glm::vec2 dir = { 0.f,0.f };
     
    if (!m_bEnemyGoing)
    {
        switch (rand_dir)
        {
        case 0:
            nextX = range + cur_enemy_grid.x;
            dir = { 1.f,0.f };
            break;
        case 1:
            nextY = range + rand_dir;
            dir = { 0.f,-1.f };
            break;
        case 2:
            nextX = -range + cur_enemy_grid.x;
            dir = { 0.f,-1.f };
            break;
        case 3:
            nextY = -range + rand_dir;
            dir = { 0.f,1.f, };
            break;
        default:
            break;
        }
    }    
    m_bEnemyGoing = true;
    glm::vec2 cur_grid = enemy_trs->GetGridByScreenPos();
    enemy_trs->AddPosition(dir);

    if (cur_grid.x == nextX && cur_grid.y ==nextY)
    {        
        m_bEnemyGoing = false;
        nextX = cur_grid.x;
        nextY = cur_grid.y;
    }
}

void IdleState::Exit()
{
}