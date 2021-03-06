/*
	Author: Jarrod Dowsey
	Description: Physics class
*/

#ifndef	MY_PHYSX_H
#define MY_PHYSX_H

#include "MyPhysx.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include "FreeCamera.h"
#include <vector>

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "ParticleEmitter.h"
#include "ParticleFluidEmitter.h"

#include "Ragdoll.h"
#include "PlayerController.h"
#include "TriggerVolume.h"

using namespace physx;

class MyAllocator : public PxAllocatorCallback
{
public:
	virtual ~MyAllocator() {}

	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}

	virtual void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}

private:

};


class Utilities;
class MyPhysx
{
public:
	MyPhysx(GLFWwindow* window);
	~MyPhysx();

	void SetUpPhysx();

	void UpdatePhysx(float deltaTime);

	void SetUpVisualDebugger();
	
	void SetUpTutorial();

	void Update(float deltaTime);

	void Draw();

	void InitializeParticles();

	glm::mat4 TransformToMat4(PxTransform transform);

	PxFoundation* GetFoundation() { return m_physicsFoundation; }
	PxPhysics* GetPhysics() { return m_physics; }
	PxScene* GetScene() { return m_physicsScene; }
	PxMaterial* GetPhysicsMaterial() { return m_physicsMaterial; }
	PxMaterial* GetBoxMaterial() { return m_boxMaterial; }
	PxCooking* GetPhysicsCooker() { return m_physicsCooker; }

private:

	PxFoundation* m_physicsFoundation;
	PxPhysics* m_physics;
	PxScene* m_physicsScene;
	PxDefaultErrorCallback m_defaultErrorCallback;
	PxDefaultAllocator m_defaultAllocatorCallback;
	PxMaterial* m_physicsMaterial;
	PxMaterial* m_boxMaterial;
	PxCooking* m_physicsCooker;
	PxSimulationFilterShader m_defaultFilterShader = PxDefaultSimulationFilterShader;

	PxBoxGeometry m_box;
	PxBoxGeometry m_wall;
	PxBoxGeometry m_wall2;
	PxBoxGeometry m_wall3;
	PxSphereGeometry m_ball;

	PxRigidStatic* m_staticWall;
	PxRigidStatic* m_staticWall2;
	PxRigidStatic* m_staticWall3;
	PxRigidStatic* m_staticWall4;
	PxRigidStatic* m_staticWall5;
	PxRigidStatic* m_staticWall6;
	PxRigidStatic* m_staticWall7;
	PxRigidDynamic* m_dynamicActor;
	PxRigidDynamic* m_dynamicBall;

	Ragdoll* m_ragdoll;
	PlayerController* m_player;
	TriggerVolume* m_trigger;
	PxRigidActor* m_triggerVolume;
	PxShape* box;

	std::vector<PxRigidActor*> m_actors;

	FreeCamera* m_camera;

	ParticleFluidEmitter* m_emitter;
	PxParticleFluid* particleSystem;
	PxU32 maxParticles;

	glm::mat4 m_transform;

};

#endif