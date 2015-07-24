#include "Spring.h"
#include <Gizmos.h>

Spring::Spring(RigidBody* rigid1, RigidBody* rigid2) : PhysicsObject()
{
	m_rigidBody1 = rigid1;
	m_rigidBody2 = rigid2;
	m_springCoefficient = 1.0f;
}

Spring::~Spring()
{

}

void Spring::Update(const glm::vec3& gravity, float timeStep)
{
	glm::vec3 direction = m_rigidBody1->m_position - m_rigidBody2->m_position;
	direction *= -m_springCoefficient;

	m_rigidBody1->ApplyForce(direction);
	m_rigidBody2->ApplyForce(-direction);
}

void Spring::Draw()
{
	Gizmos::addLine(m_rigidBody1->m_position, m_rigidBody2->m_position, glm::vec4(1, 0, 0, 1));
}