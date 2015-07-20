#include "PhysicsScene.h"
#include <Gizmos.h>
#include "Sphere.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

#include "FreeCamera.h"

PhysicsScene::PhysicsScene(GLFWwindow* window)
{
	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(50, 50, 50), vec3(0), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

	m_gravity = glm::vec3(0, -10, 0);

	Gizmos::create();

	for (int i = 0; i < 5; i++)
	{
		AddActor(new Sphere(glm::vec3(15 * i, 0, 0), glm::vec3(0, 10, 0) * m_gravity, 3.0f, 5.0f, glm::vec4(1,0,0,1)));
	}
}

PhysicsScene::~PhysicsScene()
{

}

void PhysicsScene::Update(float deltaTime)
{
	m_camera->Update(deltaTime);

	for each (Sphere* object in actors)
	{
		object->Update(deltaTime);
	}
}

void PhysicsScene::Draw()
{
	Gizmos::clear();

	for each (Sphere* object in actors)
	{
		Gizmos::addSphere(object->m_position, object->m_radius, 15, 15, object->m_colour);
	}

	Gizmos::draw(m_camera->GetProjectionView());
}

void PhysicsScene::AddActor(PhysicsObject* physicsObject)
{
	actors.push_back(physicsObject);
}

void PhysicsScene::RemoveActor(PhysicsObject* physicsObject)
{

}
