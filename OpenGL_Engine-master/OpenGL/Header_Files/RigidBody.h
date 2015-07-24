/*
	Author: Jarrod Doswey
	Description: Rigid body class
*/

#ifndef	RIGID_BODY_H
#define RIGID_BODY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(const glm::vec3& position, const glm::vec3& velocity, const glm::quat& rotation, float mass, float friction);
	~RigidBody();

	virtual void Update(const glm::vec3& gravity, float timeStep);
	virtual void Draw();

	void ApplyForce(glm::vec3 force);
	void ApplyForceToActor(RigidBody* actor2, glm::vec3 force);

	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::quat m_rotation3D;
	float m_mass;
	float m_dynamicFrictionCoefficient;


private:

};
#endif