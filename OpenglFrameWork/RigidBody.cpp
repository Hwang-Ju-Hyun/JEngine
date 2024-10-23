#include "RigidBody.h"
#include "GameObject.h"
#include "TimeManager.h"

RigidBody::RigidBody(GameObject* _owner) 
	:BaseComponent(_owner)
{

}

RigidBody::~RigidBody()
{
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
	double dt=TimeManager::GetInstance()->GetDeltaTime();

	m_vVelocity.x = m_vVelocity.x + m_vAccelation.x * dt;
	m_vVelocity.y = m_vVelocity.y + m_vAccelation.y * dt;
}
