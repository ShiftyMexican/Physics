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
#include "FreeCamera.h"

class PhysicsScene
{
public:
	PhysicsScene(GLFWwindow* window);
	~PhysicsScene();

	glm::vec3 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> actors;

	void AddActor(PhysicsObject* physicsObject);
	void RemoveActor(PhysicsObject* physicsObject);
	void Update(float deltaTime);
	void Draw();

	bool SphereToPlaneCollision(PhysicsObject* object1, PhysicsObject* object2);
	bool SphereToSphereCollision(PhysicsObject* object1, PhysicsObject* object2);
	bool SphereToAABBCollision(PhysicsObject* object1, PhysicsObject* object2);

private:

	FreeCamera* m_camera;
	GLFWwindow* m_window;
	float m_timer;

};
#endif