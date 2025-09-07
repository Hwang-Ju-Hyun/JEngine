#include "Prefabs.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "Serializer.h"
#include "CollisionManager.h"
#include "Bomb.h"
#include "BombFragment.h"
#include "Collision.h"
#include "ComponentManager.h"
#include <iostream>


Prefabs::Prefabs()
{

}

Prefabs::~Prefabs()
{

}

Bomb* Prefabs::CreateBombs(const std::string& _path, GameObject* _bombOwner)
{    
    Transform* owner_trs = dynamic_cast<Transform*>(_bombOwner->FindComponent(Transform::TransformTypeName));
    assert(owner_trs!=nullptr);
    GameObject* bomb_obj = Serializer::GetInstance()->LoadJson(_path,true); 
    assert(bomb_obj != nullptr);
    Transform* bomb_trs = dynamic_cast<Transform*>(bomb_obj->FindComponent(Transform::TransformTypeName));
    assert(bomb_trs != nullptr);
    Sprite* bomb_spr = dynamic_cast<Sprite*>(bomb_obj->FindComponent(Sprite::SpriteTypeName));
    assert(bomb_trs != nullptr);
    Bomb* bomb_comp = dynamic_cast<Bomb*>(bomb_obj->FindComponent(Bomb::BombTypeName));
    assert(bomb_comp!=nullptr);
    bomb_trs->SetPosition(owner_trs->GetPosition());
    bomb_trs->SetScale({ 66.f,50.f });
    bomb_obj->SetModelType(MODEL_TYPE::RECTANGLE);
    bomb_comp->SetRemainTime(1.f);
    
    return bomb_comp;
}

BombFragment* Prefabs::CreateBombFragment(const std::string& _path, Bomb* _bomb)
{
    GameObject* bomb_frag_obj = Serializer::GetInstance()->LoadJson(_path, true);
    BombFragment* bomb_frag_comp = static_cast<BombFragment*>(bomb_frag_obj->FindComponent(BombFragment::BombFragmentTypeName));    
    bomb_frag_obj->AddComponent(Collision::CollisionTypeName, new Collision(bomb_frag_obj));
    bomb_frag_obj->SetModelType(MODEL_TYPE::RECTANGLE);

    return bomb_frag_comp;
}
