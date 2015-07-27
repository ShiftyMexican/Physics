#include "MyPhysx.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <Gizmos.h>
#include "FreeCamera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
using namespace physx;

MyPhysx::MyPhysx(GLFWwindow* window)
{

	SetUpPhysx();
	SetUpVisualDebugger();
	SetUpTutorial();

	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(0, 50, 200), vec3(0, 20, 0), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

}

MyPhysx::~MyPhysx()
{

}

void MyPhysx::Update(float deltaTime)
{
	m_camera->Update(deltaTime);
	UpdatePhysx(deltaTime);
}

void MyPhysx::Draw()
{
	Gizmos::clear();

	glm::mat4 newRot = TransformToMat4(m_dynamicActor->getGlobalPose());

	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(100, 0.0001, 100), glm::vec4(0.25, 0.25, 0.25, 1), nullptr);
	Gizmos::addAABBFilled(glm::vec3(m_dynamicActor->getGlobalPose().p.x, m_dynamicActor->getGlobalPose().p.y, m_dynamicActor->getGlobalPose().p.z), glm::vec3(2, 2, 2), glm::vec4(0, 0.7, 0.7, 1), &newRot);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall->getGlobalPose().p.x, m_staticWall->getGlobalPose().p.y, m_staticWall->getGlobalPose().p.z), glm::vec3(5, 50, 100), glm::vec4(1, 0, 0, 1));
	Gizmos::addAABBFilled(glm::vec3(m_staticWall2->getGlobalPose().p.x, m_staticWall2->getGlobalPose().p.y, m_staticWall2->getGlobalPose().p.z), glm::vec3(5, 50, 100), glm::vec4(1, 0, 0, 1));

	Gizmos::draw(m_camera->GetProjectionView());
}

void MyPhysx::SetUpPhysx()
{
	PxAllocatorCallback *myCallback = new MyAllocator();
	m_physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback, m_defaultErrorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_physicsFoundation, PxTolerancesScale());
	PxInitExtensions(*m_physics);

	// Create physics material
	m_physicsMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.5f);
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_physicsScene = m_physics->createScene(sceneDesc);
}

void MyPhysx::UpdatePhysx(float deltaTime)
{
	if (deltaTime <= 0)
		return;

	m_physicsScene->simulate(deltaTime);
	while (m_physicsScene->fetchResults() == false)
	{

	}
}

void MyPhysx::SetUpVisualDebugger()
{
	if (m_physics->getPvdConnectionManager() == NULL)
		return;

	const char* pvd_host_ip = "127.0.0.1";
	int port = 5425;
	unsigned int timeout = 100;
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	auto theConnection = PxVisualDebuggerExt::createConnection(m_physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void MyPhysx::SetUpTutorial()
{
	// Add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*m_physics, pose, PxPlaneGeometry(), *m_physicsMaterial);

	// Add a Box
	m_boxMaterial = m_physics->createMaterial(0.1f, 0.4f, 1.0f);
	PxReal boxDensity = 2.0f;
	m_box = PxVec3(2, 2, 2);
	PxTransform transform(PxVec3(0, 100, 0));
	m_dynamicActor = PxCreateDynamic(*m_physics, transform, m_box, *m_boxMaterial, boxDensity);
	m_dynamicActor->setLinearVelocity(PxVec3(-50, 0, 0));
	m_dynamicActor->setAngularDamping(0.2f);
	m_dynamicActor->setLinearDamping(0.01f);
	m_dynamicActor->setMass(1);

	// Adding a wall
	m_wall = PxVec3(5, 50, 100);
	PxTransform wallTransform(PxVec3(-100, 50, 0));
	m_staticWall = PxCreateStatic(*m_physics, wallTransform, m_wall, *m_boxMaterial);

	// Adding a wall
	m_wall = PxVec3(5, 50, 100);
	PxTransform wallTransform2(PxVec3(100, 50, 0));
	m_staticWall2 = PxCreateStatic(*m_physics, wallTransform2, m_wall, *m_boxMaterial);

	// Add Actors to the scene
	m_physicsScene->addActor(*plane);
	m_physicsScene->addActor(*m_dynamicActor);
	m_physicsScene->addActor(*m_staticWall);
	m_physicsScene->addActor(*m_staticWall2);

	vec3 pos = vec3(m_dynamicActor->getGlobalPose().p.x, m_dynamicActor->getGlobalPose().p.y, m_dynamicActor->getGlobalPose().p.z);
	std::cout << "Position: " << pos.x << " , " << pos.y << " , " << pos.z << "\n";

}

glm::mat4 MyPhysx::TransformToMat4(PxTransform transform)
{
	return mat4_cast(quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z));
}
