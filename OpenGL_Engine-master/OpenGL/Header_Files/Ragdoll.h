/*
	Author: Jarrod Dowsey
	Description: Ragdoll class
*/

#ifndef	RAGDOLL_H
#define RAGDOLL_H

#include <PxPhysicsAPI.h>
#include <PxScene.h>

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace physx;

enum RagdollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM,

};

struct RagdollNode
{
	PxQuat globalRotation;

	PxVec3 scaledGlobalPos;

	int parentNodeIDx;
	float halfLength;
	float radius;
	float parentLinkPos;

	float childLinkPos;
	char* name;

	PxArticulationLink* linkPtr;

	// Consturctor
	RagdollNode(PxQuat _globalRotation, int _parentNodeIDx, float _halfLength, float _radius, float _parentLinkPos, float _childLinkPos, char* _name)
	{
		globalRotation = _globalRotation;
		parentLinkPos = _parentLinkPos;
		halfLength = _halfLength;
		radius = _radius;
		parentNodeIDx = _parentNodeIDx;
		childLinkPos = _childLinkPos;
		name = _name;
	}
};

class Ragdoll
{
public:
	Ragdoll(PxPhysics* physicd, PxMaterial* material, PxScene* scene);
	~Ragdoll();

	const PxVec3 X_AXIS = PxVec3(1, 0, 0);
	const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
	const PxVec3 Z_AXIS = PxVec3(0, 0, 1);

	void Update(float deltaTime);
	void Draw();

	PxArticulation* MakeRagdoll(PxPhysics* _physics, RagdollNode** _nodeArray, PxTransform _worldPos, float _scaleFactor, PxMaterial* ragdollMaterial);

private:

	PxArticulation* m_ragdoll;

};

#endif