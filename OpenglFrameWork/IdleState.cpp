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
    std::cout << cur_enemy_grid.x << "," << cur_enemy_grid.y << std::endl;
    
    int nextX=cur_enemy_grid.x;
    int nextY=cur_enemy_grid.y;
    switch (rand_dir)
    {
    case 0:
        nextX = range+cur_enemy_grid.x;        
        break;
    case 1:        
        nextY = range + rand_dir;    
        break;
    case 2:
        nextX = -range + cur_enemy_grid.x;
        break;
    case 3:
        nextY = -range + rand_dir;
        break;
    default:
        break;
    }

    float dt=TimeManager::GetInstance()->GetDeltaTime();
    glm::vec2 dir;
    if (nextX > 0 && nextY > 0)
        dir = { 1.f,0.f };
    else if (nextX < 0 && nextY >0)
        dir = { -1.f,0.f };
    else if (nextX > 0 && nextY < 0)
        dir = { 0.f,-1.f };
    else
        dir = { 0.f,1.f, };

    enemy_trs->AddPosition(dir);
}

void IdleState::Exit()
{
}