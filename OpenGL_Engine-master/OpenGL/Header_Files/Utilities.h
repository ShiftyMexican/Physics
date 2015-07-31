/*
	Author: Jarrod Dowsey	
	Description: Utilities class for extra functions to use everywhere
*/

#ifndef	UTILITIES_H
#define UTILITIES_H

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <vector>

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace physx;

class Utilities
{
public:
	Utilities();
	~Utilities();


	// Add Widget function as well as add shape functions
	static void AddWidget(PxShape* _shape, PxRigidActor* _actor);

	static void AddBox(PxShape* _pShape, PxRigidActor* _actor);

	static void AddSphere(PxShape* _pShape, PxRigidActor* _actor);

	static void AddCapsule(PxShape* _pShape, PxRigidActor* _actor);

private:

};

#endif