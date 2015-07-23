#include "RigidBody.h"

RigidBody::RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass) :
	m_position(position),
	m_velocity(velocity),
	m_rotation3D(rotation),
	m_mass(mass)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::Update(glm::vec3 gravity, float timeStep)
{
	m_velocity += gravity;
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