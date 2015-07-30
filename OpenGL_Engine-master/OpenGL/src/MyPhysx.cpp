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

#include "ParticleEmitter.h"
#include "ParticleFluidEmitter.h"

#include "Ragdoll.h"

using namespace glm;
using namespace physx;

MyPhysx::MyPhysx(GLFWwindow* window)
{

	SetUpPhysx();
	SetUpVisualDebugger();
	SetUpTutorial();

	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(0, 10, 40), vec3(0, 0, 0), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

	PxParticleFluid* particleSystem;

	PxU32 maxParticles = 600;
	bool perParticleRestOffset = false;
	particleSystem = m_physics->createParticleFluid(maxParticles, perParticleRestOffset);
	
	particleSystem->setRestParticleDistance(0.3f);
	particleSystem->setDynamicFriction(0.1f);
	particleSystem->setStaticFriction(0.1f);
	particleSystem->setDamping(0.1f);
	particleSystem->setParticleMass(0.1f);
	particleSystem->setRestitution(0);

	particleSystem->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	particleSystem->setStiffness(100);

	if (particleSystem)
	{
		m_physicsScene->addActor(*particleSystem);
		m_emitter = new ParticleFluidEmitter(maxParticles, PxVec3(0, 5, 0), particleSystem, 0.01f);
		m_emitter->setStartVelocityRange(-0.001f, -250.0f, -0.001f, 0.001f, -250.0f, 0.001f);
	}

	m_ragdoll = new Ragdoll(m_physics, m_physicsMaterial, m_physicsScene);

}

MyPhysx::~MyPhysx()
{

}

void MyPhysx::Update(float deltaTime)
{
	m_camera->Update(deltaTime);
	UpdatePhysx(deltaTime);

	m_ragdoll->Update(deltaTime);

	if (m_emitter)
	{
		m_emitter->update(deltaTime);
	}

}

void MyPhysx::Draw()
{
	Gizmos::clear();

	glm::mat4 newRot = TransformToMat4(m_dynamicActor->getGlobalPose());
	glm::mat4 ballRot = TransformToMat4(m_dynamicBall->getGlobalPose());

	Gizmos::addAABBFilled(glm::vec3(0, 0.0001, 0), glm::vec3(40, 0.0001, 40), glm::vec4(0.25, 0.25, 0.25, 1), nullptr);
	Gizmos::addAABBFilled(glm::vec3(m_dynamicActor->getGlobalPose().p.x, m_dynamicActor->getGlobalPose().p.y, m_dynamicActor->getGlobalPose().p.z), glm::vec3(2, 2, 2), glm::vec4(0, 0.7, 0.7, 1), &newRot);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall->getGlobalPose().p.x, m_staticWall->getGlobalPose().p.y, m_staticWall->getGlobalPose().p.z), glm::vec3(1, .2f, 2), glm::vec4(1, 1, 1, 1), nullptr, false);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall2->getGlobalPose().p.x, m_staticWall2->getGlobalPose().p.y, m_staticWall2->getGlobalPose().p.z), glm::vec3(1, .2f, 2), glm::vec4(1, 1, 1, 1), nullptr, false);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall3->getGlobalPose().p.x, m_staticWall3->getGlobalPose().p.y, m_staticWall3->getGlobalPose().p.z), glm::vec3(3, .2f, 1), glm::vec4(1, 1, 1, 1), nullptr, false);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall4->getGlobalPose().p.x, m_staticWall4->getGlobalPose().p.y, m_staticWall4->getGlobalPose().p.z), glm::vec3(3, .2f, 1), glm::vec4(1, 1, 1, 1), nullptr, false);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall5->getGlobalPose().p.x, m_staticWall5->getGlobalPose().p.y, m_staticWall5->getGlobalPose().p.z), glm::vec3(5, 20, 40), glm::vec4(1, 1, 1, 1), nullptr, false);
	Gizmos::addAABBFilled(glm::vec3(m_staticWall6->getGlobalPose().p.x, m_staticWall6->getGlobalPose().p.y, m_staticWall6->getGlobalPose().p.z), glm::vec3(5, 20, 40), glm::vec4(1, 1, 1, 1), nullptr, false);
	Gizmos::addSphere(glm::vec3(m_dynamicBall->getGlobalPose().p.x, m_dynamicBall->getGlobalPose().p.y, m_dynamicBall->getGlobalPose().p.z), 2.0f, 10, 10, glm::vec4(0, 0, 1, 1), &ballRot);

	if (m_emitter)
	{
		m_emitter->renderParticles();
	}

	m_ragdoll->Draw();

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
	PxReal boxDensity = 2.0f;
	m_box = PxVec3(2, 2, 2);
	PxTransform transform(PxVec3(-30, 80, 0));
	m_dynamicActor = PxCreateDynamic(*m_physics, transform, m_box, *m_physicsMaterial, boxDensity);

	// Add a Ball
	PxTransform transform2(PxVec3(0, 30, 0));
	m_ball = 2;
	m_dynamicBall = PxCreateDynamic(*m_physics, transform2, m_ball, *m_physicsMaterial, boxDensity);
	m_dynamicBall->setLinearVelocity(PxVec3(50, 0, 0));

	// Adding a wall
	m_wall = PxVec3(1, .2f, 2);
	PxTransform wallTransform(PxVec3(-3, .2f, 0));
	m_staticWall = PxCreateStatic(*m_physics, wallTransform, m_wall, *m_physicsMaterial);

	// Adding a wall
	m_wall = PxVec3(1, .2f, 2);
	PxTransform wallTransform2(PxVec3(3, .2f, 0));
	m_staticWall2 = PxCreateStatic(*m_physics, wallTransform2, m_wall, *m_physicsMaterial);

	// Adding a wall
	m_wall2 = PxVec3(3, .2f, 1);
	PxTransform wallTransform3(PxVec3(0, .2f, 3));
	m_staticWall3 = PxCreateStatic(*m_physics, wallTransform3, m_wall2, *m_physicsMaterial);

	// Adding a wall
	m_wall2 = PxVec3(3, .2f, 1);
	PxTransform wallTransform4(PxVec3(0, .2f, -3));
	m_staticWall4 = PxCreateStatic(*m_physics, wallTransform4, m_wall2, *m_physicsMaterial);

	// Adding a wall
	m_wall3 = PxVec3(5, 20, 40);
	PxTransform wallTransform5(PxVec3(40, 20, 0));
	m_staticWall5 = PxCreateStatic(*m_physics, wallTransform5, m_wall3, *m_physicsMaterial);

	// Adding a wall
	m_wall3 = PxVec3(5, 20, 40);
	PxTransform wallTransform6(PxVec3(-40, 20, 0));
	m_staticWall6 = PxCreateStatic(*m_physics, wallTransform6, m_wall3, *m_physicsMaterial);

	// Adding a ragdoll


	// Add Actors to the scene
	m_physicsScene->addActor(*plane);
	m_physicsScene->addActor(*m_dynamicActor);
	m_physicsScene->addActor(*m_dynamicBall);
	m_physicsScene->addActor(*m_staticWall);
	m_physicsScene->addActor(*m_staticWall2);
	m_physicsScene->addActor(*m_staticWall3);
	m_physicsScene->addActor(*m_staticWall4);
	m_physicsScene->addActor(*m_staticWall5);
	m_physicsScene->addActor(*m_staticWall6);

	vec3 pos = vec3(m_dynamicActor->getGlobalPose().p.x, m_dynamicActor->getGlobalPose().p.y, m_dynamicActor->getGlobalPose().p.z);
	std::cout << "Position: " << pos.x << " , " << pos.y << " , " << pos.z << "\n";

}

glm::mat4 MyPhysx::TransformToMat4(PxTransform transform)
{
	return mat4_cast(quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z));
}
