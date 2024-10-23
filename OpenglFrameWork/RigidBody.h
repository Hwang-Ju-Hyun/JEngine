#pragma once
#include "single.h"
#include "BaseComponent.h"
#include <glm.hpp>

class RigidBody :
    public BaseComponent
{    
    RigidBody(GameObject* _owner);
    virtual ~RigidBody()override;
private:
    glm::vec2 m_vVelocity = { 0.f,0.f };
    glm::vec2 m_vAccelation = { 0.f,0.f };
public:
    glm::vec2 GetVelocity()const;
    void SetAccelation(glm::vec2 _accelation);
    glm::vec2 GetAccelation()const;
public:
    virtual void Update()override;
};

