#include "PhysicsScene.h"
#include <Gizmos.h>
#include "Sphere.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

#include "FreeCamera.h"
#include "Plane.h"

PhysicsScene::PhysicsScene(GLFWwindow* window)
{
	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(150, 10, 150), vec3(0,1,0), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

	m_gravity = glm::vec3(0, -10, 0);

	Gizmos::create();

	for (int i = 0; i < 5; i++)
	{
		AddActor(new Sphere(glm::vec3(15 * i, 20, 0), glm::vec3(0, 10, 0) * m_gravity, 3.0f, 5.0f, glm::vec4(1,0,0,1)));
	}

	AddActor(new Plane());
}

PhysicsScene::~PhysicsScene()
{

}

void PhysicsScene::Update(float deltaTime)
{
	m_camera->Update(deltaTime);

	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		(*it)->Update(m_gravity, deltaTime);

		if ((*it)->_shapeID == ShapeType::SHPERE) 
		{
			for (auto it2 = actors.begin(); it2 != actors.end(); ++it2)
			{
				if ((*it2)->_shapeID == ShapeType::PLANE)
				{
					SphereToPlaneCollision((*it), (*it2));
				}
			}
		}
	}
}

void PhysicsScene::Draw()
{
	Gizmos::clear();

	for each (PhysicsObject* object in actors)
	{
		object->Draw();
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

bool PhysicsScene::SphereToPlaneCollision(PhysicsObject* object1, PhysicsObject* object2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(object1);
	Plane* plane = dynamic_cast<Plane*>(object2);

	// If Success
	if (sphere != NULL && plane != NULL)
	{
		glm::vec3 collisionNormal = plane->GetNormal();
		float sphereToPlane = glm::dot(sphere->m_position, plane->GetNormal()) - plane->GetDistance();

		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->m_radius - sphereToPlane;

		if (intersection > 0)
		{
			glm::vec3 relativeVelocity = sphere->m_velocity;
			glm::vec3 planeNormal = collisionNormal;
			glm::vec3 forceVector = -1 * sphere->m_mass * planeNormal * (glm::dot(planeNormal, sphere->m_velocity));

			sphere->ApplyForce(forceVector * glm::vec3(2));
			sphere->m_position += collisionNormal * intersection * 0.5f;

			return true;
		}
	}
	return false;
}
