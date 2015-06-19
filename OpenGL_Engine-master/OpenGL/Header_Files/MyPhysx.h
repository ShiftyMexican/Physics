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

using namespace physx;

class MyAllocator : public PxAllocatorCallback
{
public:
	
	virtual ~MyAllocator();

	virtual void* Allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}

	virtual void Deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}

private:

};



class MyPhysx
{
public:
	MyPhysx();
	~MyPhysx();

private:

	PxFoundation* m_physicsFoundation;
	PxPhysics* m_physics;
	PxScene* m_physicsScene;
	PxDefaultErrorCallback m_defaultErrorCallback;
	PxDefaultAllocator m_defaultAllocatorCallback;
	PxSimulationFilterShader m_defaultFilterShader = PxDefaultSimulationFilterShader;


};

#endif