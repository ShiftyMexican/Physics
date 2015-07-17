#include "MyDIYPhysics.h"
#include "PhysicsScene.h"
#include "Sphere.h"

MyDIYPhysics::MyDIYPhysics()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->gravity = glm::vec3(0, -10, 0);
	m_physicsScene->timeStep = 0.001f;

	// Add 4 Balls to our simulation
	Sphere* newBall;
	newBall = new Sphere(glm::vec3(-40, 0, 0), glm::vec3(0, 0, 0), 3.0f, 1.0f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(newBall);
}

MyDIYPhysics::~MyDIYPhysics()
{

}
