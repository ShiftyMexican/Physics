/*
	Author: Jarrod Dowsey
	Description: Sphere class for physics
*/

#ifndef	SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	Sphere(glm::vec3 position, glm::vec3 velocity, float mass, float radius, glm::vec4 colour);
	~Sphere();

	void Update(float deltaTime);
	void Draw();

	float m_radius;
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_mass;
	glm::vec4 m_colour;

private:

};

#endif