/*
Author: Jarrod Dowsey
Description: Box class for physics
*/

#ifndef	BOX_H
#define BOX_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& extents, float mass, const glm::vec4& colour, float friction);
	~Box();

	void Draw();

	glm::vec3 m_extents;
	glm::vec4 m_colour;

private:

};

#endif
