#include "RigidBody.h"

RigidBody::RigidBody(const glm::vec3& position, const glm::vec3& velocity, const glm::quat& rotation, float mass, float friction) :
	m_position(position),
	m_velocity(velocity),
	m_rotation3D(rotation),
	m_mass(mass),
	m_dynamicFrictionCoefficient(friction)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::Update(const glm::vec3& gravity, float timeStep)
{
	m_velocity += gravity;
	m_velocity.x *= m_dynamicFrictionCoefficient;
	m_velocity.y *= m_dynamicFrictionCoefficient;
	m_velocity.z *= m_dynamicFrictionCoefficient;
	m_position += m_velocity * timeStep;
}

void RigidBody::Draw()
{

}

void RigidBody::ApplyForce(glm::vec3 force)
{
	m_velocity += force / m_mass;
}

void RigidBody::ApplyForceToActor(RigidBody* actor2, glm::vec3 force)
{

}