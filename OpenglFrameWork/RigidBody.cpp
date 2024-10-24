#include "RigidBody.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include <iostream>

RigidBody::RigidBody(GameObject* _owner) 
	:BaseComponent(_owner)
{

}

RigidBody::~RigidBody()
{
}


void RigidBody::SetMaxVelocity(glm::vec2 _maxVelocity)
{
    m_vMaxVelocity = _maxVelocity;
}

glm::vec2 RigidBody::GetMaxVelocity() const
{
    return m_vMaxVelocity;
}

void RigidBody::SetVelocity(glm::vec2 _velocity)
{
    m_vVelocity = _velocity;
}

glm::vec2 RigidBody::GetVelocity() const
{
	return m_vVelocity;
}

void RigidBody::SetAccelation(glm::vec2 _accelation)
{
	m_vAccelation = _accelation;
}

glm::vec2 RigidBody::GetAccelation() const
{
	return m_vAccelation;
}

void RigidBody::Update()
{	
    double dt = TimeManager::GetInstance()->GetDeltaTime();
    m_vVelocity.x = m_vVelocity.x + m_vAccelation.x * dt;
    if (m_vMaxVelocity.x < std::fabs(m_vVelocity.x))
    {
        if(m_vVelocity.x>=0)
            m_vVelocity.x = m_vMaxVelocity.x;
        else
            m_vVelocity.x = -m_vMaxVelocity.x;
    }

    m_vVelocity.y = m_vVelocity.y + m_vAccelation.y * dt;
    if (m_vMaxVelocity.y < m_vVelocity.y)
    {
        if (m_vVelocity.y >= 0)
            m_vVelocity.y = m_vMaxVelocity.y;
        else
            m_vVelocity = -m_vMaxVelocity;
    }
 
    Transform* transform = (Transform*)m_pOwner->FindComponent("Transform");
    if (transform)
    {
        glm::vec2 position = transform->GetPosition();
        position += (m_vVelocity * (float)dt); 

        if (m_vAccelation.x >0.005f)
            m_vAccelation.x -= dt;
        else if (m_vAccelation.x < 0.005f)
            m_vAccelation.x += dt;
        else
            m_vAccelation.x = 0;

        if (m_vVelocity.x > 0.005f)
            m_vVelocity.x -= dt;
        else if (m_vVelocity.x < -0.005f)
            m_vVelocity.x += dt;
        else
            m_vVelocity.x = 0;
        transform->SetPosition(position);
    }
}

void RigidBody::LoadFromJson(const json& _str)
{
    auto comp_data = _str.find("CompData");
    if (comp_data != _str.end())
    {
        auto velocity = comp_data->find("Velocity");
        m_vVelocity.x = velocity->begin().value();
        m_vVelocity.y = (velocity->begin() + 1).value();

        auto accelation = comp_data->find("Accelation");
        m_vAccelation.x = accelation->begin().value();
        m_vAccelation.y = (accelation->begin() + 1).value();

        auto max_velocity = comp_data->find("Max Velocity");
        m_vMaxVelocity.x = max_velocity->begin().value();
        m_vMaxVelocity.y = (max_velocity->begin()+1).value();

    }
}

json RigidBody::SaveToJson(const json& _str)
{
    json data;

    data["Type"] = "RigidBody";

    json comp_data;
    comp_data["Velocity"] = { m_vVelocity.x,m_vVelocity.y };
    comp_data["Accelation"] = { m_vAccelation.x,m_vAccelation.y };
    comp_data["Max Velocity"] = { m_vMaxVelocity.x,m_vMaxVelocity.y };
    data["CompData"] = comp_data;

    return data;
}

BaseRTTI* RigidBody::CreateRigidBodyComponent()
{
    GameObject* last_obj = GameObjectManager::GetInstance()->GetLastObject();
    BaseRTTI* p = last_obj->AddComponent(RigidBody::RigidBodyTypeName, new RigidBody(last_obj));
    if (p == nullptr)
    {
        std::cerr << "Error : BaseRTTI is nullptr - Sprite::CreateSpriteComponent" << std::endl;
        return nullptr;
    }
    return p;
}
