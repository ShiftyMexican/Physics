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



class MyPhysx
{
public:
	MyPhysx(GLFWwindow* window);
	~MyPhysx();

	void SetUpPhysx();

	void UpdatePhysx(float deltaTime);
	
	void SetUpTutorial();

	void Update(float deltaTime);

	void Draw();

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

	FreeCamera* m_camera;

};

#endif