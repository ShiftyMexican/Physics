#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour) :
	RigidBody(position, velocity, m_rotation3D, mass)
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