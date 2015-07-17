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
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass);
	~RigidBody();

	virtual void Update(glm::vec3 gravity, float timeStep);
	virtual void Draw();

	void ApplyForce(glm::vec3 force);
	void ApplyForceToActor(RigidBody* actor2, glm::vec3 force);

	glm::vec3 position;
	glm::vec3 velocity;
	glm::quat rotation3D;
	float mass;


private:

};
#endif