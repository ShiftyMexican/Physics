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
	Sphere(const glm::vec3& position, const glm::vec3& velocity, float mass, float radius, const glm::vec4& colour, float friction);
	~Sphere();

	void Draw();

	float m_radius;
	glm::vec4 m_colour;

private:

};

#endif