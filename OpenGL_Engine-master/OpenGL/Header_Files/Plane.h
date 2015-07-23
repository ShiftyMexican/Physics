/*
	Author: Jarrod Dowsey
	Description: Plane class
*/

#ifndef	PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	~Plane();

	virtual void Update(glm::vec3 gravity, float deltaTime) override;
	virtual void Draw();

	glm::vec3 GetNormal() { return m_normal; }
	float GetDistance() { return m_distance; }

private:

	glm::vec3 m_normal;
	float m_distance;

};
#endif