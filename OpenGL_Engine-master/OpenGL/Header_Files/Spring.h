/*
Author: Jarrod Dowsey
Description: Spring class for physics
*/

#ifndef	SPRING_H
#define SPRING_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "RigidBody.h"
#include "PhysicsObject.h"

class Spring : public PhysicsObject
{
public:
	Spring(RigidBody* rigid1, RigidBody* rigid2);
	~Spring();

	virtual void Update(const glm::vec3& gravity, float timeStep);
	void Draw();

private:

	float m_springCoefficient;
	RigidBody* m_rigidBody1;
	RigidBody* m_rigidBody2;

};

#endif