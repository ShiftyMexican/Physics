#include "Sphere.h"

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(position, velocity, rotation3D, mass)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{

}

void Sphere::Update(float deltaTime)
{
	m_position += m_velocity * deltaTime;
}

void Sphere::Draw()
{

}