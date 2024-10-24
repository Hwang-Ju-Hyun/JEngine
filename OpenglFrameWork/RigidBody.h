#pragma once
#include "single.h"
#include "BaseComponent.h"
#include <glm.hpp>

class RigidBody :
    public BaseComponent
{    
public:
    RigidBody(GameObject* _owner);
    virtual ~RigidBody()override;
private:
    glm::vec2 m_vMaxVelocity = { 0.f,0.f };
    glm::vec2 m_vVelocity = { 0.f,0.f };
    glm::vec2 m_vAccelation = { 0.f,0.f };
public:
    void SetMaxVelocity(glm::vec2 _maxVelocity);
    glm::vec2 GetMaxVelocity()const;
    void SetVelocity(glm::vec2 _velocity);
    glm::vec2 GetVelocity()const;
    void SetAccelation(glm::vec2 _accelation);
    glm::vec2 GetAccelation()const;
public:
    virtual void Update()override;
public:
    virtual void LoadFromJson(const json& _str);
    virtual json SaveToJson(const json& _str);
public:
    static constexpr const char* RigidBodyTypeName = "RigidBody";
public:
    static BaseRTTI* CreateRigidBodyComponent();
};

