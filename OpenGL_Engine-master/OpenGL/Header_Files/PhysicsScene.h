/*
	Author: Jarrod Doswey
	Description: DIY physics scene
*/

#ifndef	PHYSICS_SCENE_H
#define PHYSICS_SCENE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "PhysicsObject.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	glm::vec3 gravity;
	float timeStep;
	std::vector<PhysicsObject*> actors;

	void AddActor(PhysicsObject*);
	void RemoveActor(PhysicsObject*);
	void Update();
	void Draw();

private:

};
#endif