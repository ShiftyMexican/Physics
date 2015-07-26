#include "PhysicsScene.h"
#include <Gizmos.h>
#include "Sphere.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

#include "FreeCamera.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"

PhysicsScene::PhysicsScene(GLFWwindow* window)
{
	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(300, 150, 300), vec3(0, 90, 0), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

	m_window = window;

	m_gravity = glm::vec3(0, -9.8, 0);

	Gizmos::create();

	// Adding the balls
	for (int i = 0; i < 5; i++)
	{
		AddActor(new Sphere(glm::vec3(15, (i + 2) * 20, 10), glm::vec3(cosf(i), 0, sinf(i)), 3.0f, 5.0f, glm::vec4(1, 0, 0, 1), 0.99f));
	}

	// Adding the Boxes/Walls
	AddActor(new Box(glm::vec3(0, 100, 95), glm::vec3(0), glm::vec3(100, 100, 5), 1000, glm::vec4(1, 1, 1, 1), 0));
	AddActor(new Box(glm::vec3(0, 100, -95), glm::vec3(0), glm::vec3(100, 100, 5), 1000, glm::vec4(1, 1, 1, 1), 0));
	AddActor(new Box(glm::vec3(95, 100, 0), glm::vec3(0), glm::vec3(5, 100, 100), 1000, glm::vec4(1, 1, 1, 1), 0));
	AddActor(new Box(glm::vec3(-95, 100, 0), glm::vec3(0), glm::vec3(5, 100, 100), 1000, glm::vec4(1, 1, 1, 1), 0));
	AddActor(new Box(glm::vec3(0, 205, 0), glm::vec3(0), glm::vec3(100, 5, 100), 1000, glm::vec4(1, 1, 1, 1), 0));
	
	// Adding the Plane
	AddActor(new Plane());

	Sphere* ball1 = new Sphere(glm::vec3(30, 30, 20), glm::vec3(10, 0, 10), 3.0f, 5.0f, glm::vec4(0, 1, 0, 1), 0.99f);
	Sphere* ball2 = new Sphere(glm::vec3(15, 30, 10), glm::vec3(-10, 0, -10), 3.0f, 5.0f, glm::vec4(0, 1, 0, 1), 0.99f);

	AddActor(new Spring(ball1, ball2));

	AddActor(ball1);
	AddActor(ball2);
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
				if ((*it) != (*it2))
				{
					if ((*it2)->_shapeID == ShapeType::PLANE)
					{
						SphereToPlaneCollision((*it), (*it2));
					}

					if ((*it2)->_shapeID == ShapeType::SHPERE)
					{
						SphereToSphereCollision((*it), (*it2));
					}

					if ((*it2)->_shapeID == ShapeType::BOX)
					{
						SphereToAABBCollision((*it), (*it2));
					}
				}
			}
		}
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
	{
		for(auto actor : actors)
		{
			if (actor->_shapeID == ShapeType::SHPERE)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(actor);
				sphere->ApplyForce(glm::vec3(5, 50, 0));
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

		float intersection = sphere->m_radius - sphereToPlane;

		if (intersection > 0)
		{
			glm::vec3 relativeVelocity = sphere->m_velocity;
			glm::vec3 planeNormal = collisionNormal;
			glm::vec3 forceVector = -1 * sphere->m_mass * planeNormal * (glm::dot(planeNormal, sphere->m_velocity));

			sphere->ApplyForce(forceVector * glm::vec3(2));
			sphere->m_position += collisionNormal * intersection;

			return true;
		}
	}
	return false;
}

bool PhysicsScene::SphereToSphereCollision(PhysicsObject* object1, PhysicsObject* object2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(object1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(object2);

	if (sphere1 != nullptr && sphere2 != nullptr) {
		float dist = glm::distance(sphere1->m_position, sphere2->m_position);
		float intersect = sphere1->m_radius + sphere2->m_radius - dist;

		if (intersect > 0) {
			glm::vec3 delta = sphere1->m_position - sphere2->m_position;
			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = sphere1->m_velocity - sphere2->m_velocity;
			glm::vec3 collisionVector = collisionNormal * glm::dot(relativeVelocity, collisionNormal);
			glm::vec3 forceVector = collisionVector * 1.0f / (1 / sphere1->m_mass + 1 / sphere2->m_mass);

			sphere1->ApplyForce(2.0f * -forceVector);
			sphere2->ApplyForce(2.0f * forceVector);

			float totalMass = sphere1->m_mass + sphere2->m_mass;

			glm::vec3 seperationVector = collisionNormal * intersect;
			sphere1->m_position += (seperationVector * (sphere2->m_mass / totalMass));
			sphere2->m_position -= (seperationVector * (sphere1->m_mass / totalMass));

			return true;
		}
	}
	return false;
}

bool PhysicsScene::SphereToAABBCollision(PhysicsObject* object1, PhysicsObject* object2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(object1);
	Box* box = dynamic_cast<Box*>(object2);

	float halfExtentsX = box->m_extents.x;
	float halfExtentsY = box->m_extents.y;
	float halfExtentsZ = box->m_extents.z;

	glm::vec3 wallTopBackLeft = box->m_position - glm::vec3(halfExtentsX, halfExtentsY, -halfExtentsZ);
	glm::vec3 wallTopBackRight = box->m_position - glm::vec3(-halfExtentsX, halfExtentsY, -halfExtentsZ);
	glm::vec3 wallTopFrontLeft = box->m_position - glm::vec3(halfExtentsX, halfExtentsY, halfExtentsZ);
	glm::vec3 wallTopFrontRight = box->m_position - glm::vec3(-halfExtentsX, halfExtentsY, halfExtentsZ);

	glm::vec3 wallBottomBackLeft = box->m_position - glm::vec3(halfExtentsX, -halfExtentsY, -halfExtentsZ);
	glm::vec3 wallBottomBackRight = box->m_position - glm::vec3(-halfExtentsX, -halfExtentsY, -halfExtentsZ);
	glm::vec3 wallBottomFrontLeft = box->m_position - glm::vec3(halfExtentsX, -halfExtentsY, halfExtentsZ);
	glm::vec3 wallBottomFrontRight = box->m_position - glm::vec3(-halfExtentsX, -halfExtentsY, halfExtentsZ);

	float intersectDistance;
	glm::vec3 collisionNormal;
	float xSmallestOverlap = 100000;
	float ySmallestOverlap = 100000;
	float zSmallestOverlap = 100000;
	float smallestOverlap = 100000;

	float wallMinX = wallBottomFrontLeft.x;
	float wallMaxX = wallBottomFrontRight.x;

	float wallMinY = wallTopFrontLeft.y;
	float wallMaxY = wallBottomBackRight.y;

	float wallMinZ = wallBottomFrontLeft.z;
	float wallMaxZ = wallBottomBackRight.z;

	float sphereMinX = sphere->m_position.x - sphere->m_radius;
	float sphereMaxX = sphere->m_position.x + sphere->m_radius;

	float sphereMinY = sphere->m_position.y - sphere->m_radius;
	float sphereMaxY = sphere->m_position.y + sphere->m_radius;

	float sphereMinZ = sphere->m_position.z - sphere->m_radius;
	float sphereMaxZ = sphere->m_position.z + sphere->m_radius;

	if (wallMaxX > sphereMinX && sphereMaxX > wallMinX)
	{
		// Collision has occured on the x-axis
		float xOverlap1 = wallMaxX - sphereMinX;
		float xOverlap2 = wallMinX - sphereMaxX;

		if (glm::abs(xOverlap1) < glm::abs(xOverlap2))
		{
			xSmallestOverlap = xOverlap1;
		}
		else
		{
			xSmallestOverlap = xOverlap2;
		}

		collisionNormal = glm::vec3(1, 0, 0);
		intersectDistance = xSmallestOverlap;
		smallestOverlap = xSmallestOverlap;
	}
	else
	{
		return false;
	}

	if (wallMaxY > sphereMinY && sphereMaxY > wallMinY)
	{
		// Collision has occured on the y-axis
		float yOverlap1 = wallMaxY - sphereMinY;
		float yOverlap2 = wallMinY - sphereMaxY;

		if (glm::abs(yOverlap1) < glm::abs(yOverlap2))
		{
			ySmallestOverlap = yOverlap1;
		}
		else
		{
			ySmallestOverlap = yOverlap2;
		}

		if (glm::abs(ySmallestOverlap) < glm::abs(smallestOverlap))
		{
			collisionNormal = glm::vec3(0, 1, 0);
			intersectDistance = ySmallestOverlap;
			smallestOverlap = ySmallestOverlap;
		}	
	}
	else
	{
		return false;
	}
		

	if (wallMaxZ > sphereMinZ && sphereMaxZ > wallMinZ)
	{
		// Collision has occured on the z-axis
		float zOverlap1 = wallMaxZ - sphereMinZ;
		float zOverlap2 = wallMinZ - sphereMaxZ;

		if (glm::abs(zOverlap1) < glm::abs(zOverlap2))
		{
			zSmallestOverlap = zOverlap1;
		}
		else
		{
			zSmallestOverlap = zOverlap2;
		}

		if (glm::abs(zSmallestOverlap) < glm::abs(smallestOverlap))
		{
			collisionNormal = glm::vec3(0, 0, 1);
			intersectDistance = zSmallestOverlap;
			smallestOverlap = zSmallestOverlap;
		}
	}
	else
	{
		return false;
	}

	
	sphere->m_position += collisionNormal * intersectDistance;
	glm::vec3 forceVector = -1 * sphere->m_mass * collisionNormal * (glm::dot(collisionNormal, sphere->m_velocity));

	sphere->ApplyForce(forceVector + forceVector);

}
