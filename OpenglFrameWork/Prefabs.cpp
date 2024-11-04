#include "Prefabs.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "Serializer.h"
#include "Bomb.h"

Prefabs::Prefabs()
{

}

Prefabs::~Prefabs()
{

}

void Prefabs::CreateBombs(const std::string& _path, GameObject* _bombOwner)
{
    Transform* owner_trs = static_cast<Transform*>(_bombOwner->FindComponent(Transform::TransformTypeName));

    GameObject* bomb_obj = Serializer::GetInstance()->LoadJson(_path,true); 
    Transform* bomb_trs = static_cast<Transform*>(bomb_obj->FindComponent(Transform::TransformTypeName));
    Sprite* bomb_spr = static_cast<Sprite*>(bomb_obj->FindComponent(Sprite::SpriteTypeName));
    Bomb* bomb_comp = static_cast<Bomb*>(bomb_obj->FindComponent(Bomb::BombTypeName));
    float explode_time = bomb_comp->GetExplodeTime();

    bomb_trs->SetPosition(owner_trs->GetPosition());
    bomb_trs->SetScale({ 50.f,50.f });
    bomb_obj->SetModelType(MODEL_TYPE::CIRCLE);
}
