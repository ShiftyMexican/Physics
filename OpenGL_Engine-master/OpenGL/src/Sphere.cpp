#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(const glm::vec3& position, const glm::vec3& velocity, float mass, float radius, const glm::vec4& colour, float friction) :
RigidBody(position, velocity, m_rotation3D, mass, friction)
{
	m_radius = radius;
	m_colour = colour;
	_shapeID = ShapeType::SHPERE;
}

Sphere::~Sphere()
{

}

void Sphere::Draw()
{
	Gizmos::addSphere(m_position, m_radius, 15, 15, m_colour);
}