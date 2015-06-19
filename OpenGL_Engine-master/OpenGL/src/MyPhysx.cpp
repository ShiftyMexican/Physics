#include "MyPhysx.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <Gizmos.h>
#include "FreeCamera.h"

using namespace physx;

MyPhysx::MyPhysx(GLFWwindow* window)
{
	SetUpPhysx();
	SetUpTutorial();

	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(50, 50, 50), vec3(0), vec3(0, 1, 0));
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

	Gizmos::addAABBFilled(glm::vec3(0, 0, 0), glm::vec3(100, 0.0001, 100), glm::vec4(0.4, 0.2, 0, 1), nullptr);

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

void MyPhysx::SetUpTutorial()
{
	// Add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*m_physics, pose, PxPlaneGeometry(), *m_physicsMaterial);
	// add it to the scene
	m_physicsScene->addActor(*plane);

	// Add a Box
	float density = 1;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(0, 5, 0));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, density);
	
	// Add it to the scene
	m_physicsScene->addActor(*dynamicActor);
}
