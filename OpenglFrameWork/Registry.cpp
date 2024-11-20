#include "Registry.h"
#include "Transform.h"
#include "Sprite.h"
#include "RigidBody.h"
#include "Player.h"
#include "Wall.h"
#include "Bomb.h"
#include "Collision.h"
#include "BombFragment.h"
#include "AI.h"

Registry::Registry()
{
    m_mapRttiMap.insert({ Transform::TransformTypeName,&Transform::CreateTransformComponent });
    m_mapRttiMap.insert({ Sprite::SpriteTypeName,&Sprite::CreateSpriteComponent });
    m_mapRttiMap.insert({ RigidBody::RigidBodyTypeName,&RigidBody::CreateRigidBodyComponent });
    m_mapRttiMap.insert({ Player::PlayerTypeName,&Player::CreatePlayerComponent });
    m_mapRttiMap.insert({ Wall::WallTypeName,&Wall::CreateWallComponent });
    m_mapRttiMap.insert({ Bomb::BombTypeName,&Bomb::CreateBombComponent});
    m_mapRttiMap.insert({ BombFragment::BombFragmentTypeName,&BombFragment::CreateBombFragComponent });
    m_mapRttiMap.insert({ AI::AITypeName,&AI::CreateAIComponent });
    //m_mapRttiMap.insert({ Collision::CollisionTypeName,&Collision::CreateCollisionComponent });
}

Registry::~Registry()
{

}

BaseRTTI* Registry::FindOrCreate(const std::string& _type)
{
    std::map<std::string, BaseRTTI* (*)()>::iterator iter =m_mapRttiMap.find(_type);
    if (iter != m_mapRttiMap.end())
    {
        if (iter->second == nullptr)
            return nullptr;
        return iter->second();
    }
    if (iter->second == nullptr)
        return nullptr;
    return nullptr;
}
