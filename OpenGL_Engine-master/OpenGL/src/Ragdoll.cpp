#include "Ragdoll.h"
#include <Gizmos.h>
#include "Utilities.h"

Ragdoll::Ragdoll(PxPhysics* physics, PxMaterial* material, PxScene* scene)
{

	RagdollNode* ragdollData[]
	{
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT, 1, 3, 1, 1, "lower spine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE, 1, 1, -1, 1, "left pelvis"),
		new RagdollNode(PxQuat(0, Z_AXIS), LOWER_SPINE, 1, 1, -1, 1, "right pelvis"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_PELVIS, 5, 2, -1, 1, "L upper leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_PELVIS, 5, 2, -1, 1, "R upper leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_UPPER_LEG, 5, 1.75, -1, 1, "L lower leg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_UPPER_LEG, 5, 1.75, -1, 1, "R lowerleg"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE, 1, 3, 1, -1, "upper spine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "left clavicle"),
		new RagdollNode(PxQuat(0, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "right clavicle"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE, 1, 1, 1, -1, "neck"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK, 1, 3, 1, -1, "HEAD"),
		new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_CLAVICLE, 3, 1.5, -1, 1, "left upper arm"),
		new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_CLAVICLE, 3, 1.5, -1, 1, "right upper arm"),
		new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_UPPER_ARM, 3, 1, -1, 1, "left lower arm"),
		new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_UPPER_ARM, 3, 1, -1, 1, "right lower arm"),
		NULL

	};

	m_ragdoll = Ragdoll::MakeRagdoll(physics, ragdollData, PxTransform(PxVec3(10, 20, 0)), 0.1f, material);
	scene->addArticulation(*m_ragdoll);
}

Ragdoll::~Ragdoll()
{

}

void Ragdoll::Update(float deltaTime)
{

}

void Ragdoll::Draw()
{
	PxU32 nLinks = m_ragdoll->getNbLinks();
	PxArticulationLink** links = new PxArticulationLink*[nLinks];
	m_ragdoll->getLinks(links, nLinks);

	while (nLinks--)
	{
		PxArticulationLink* link = links[nLinks];
		PxU32 nShapes = link->getNbShapes();
		PxShape** shapes = new PxShape*[nShapes];
		link->getShapes(shapes, nShapes);

		while (nShapes--)
		{

			//PxSphereGeometry geometry;
			//shapes[nShapes]->getSphereGeometry(geometry);
			//float radius = geometry.radius + 1;
			//glm::vec3 center;
			//center.x = link->getWorldBounds().getCenter().x;
			//center.y = link->getWorldBounds().getCenter().y;
			//center.z = link->getWorldBounds().getCenter().z;
			//glm::mat4 newRot = Utils::TransformToMat4(_actor->getGlobalPose());
			Utilities::AddWidget(shapes[nShapes], link);
		}
	}
	delete[] links;
}

PxArticulation* Ragdoll::MakeRagdoll(PxPhysics* _physics, RagdollNode** _nodeArray, PxTransform _worldPos, float _scaleFactor, PxMaterial* ragdollMaterial)
{
	PxArticulation* articulation = _physics->createArticulation();
	RagdollNode** currentNode = _nodeArray;

	while (*currentNode != NULL)
	{
		RagdollNode* currentNodePtr = *currentNode;
		RagdollNode* parentNode = nullptr;

		float radius = currentNodePtr->radius*_scaleFactor;
		float halfLength = currentNodePtr->halfLength*_scaleFactor;
		float childHalfLength = radius + halfLength;
		float parentHalfLength = 0;

		PxArticulationLink* parentLinkPtr = NULL;
		currentNodePtr->scaledGlobalPos = _worldPos.p;

		if (currentNodePtr->parentNodeIDx != -1)
		{
			parentNode = *(_nodeArray + currentNodePtr->parentNodeIDx);

			parentLinkPtr = parentNode->linkPtr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) * _scaleFactor;

			PxVec3 currentRelative = currentNodePtr->childLinkPos * currentNodePtr->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));

			PxVec3 parentRelative = -currentNodePtr->parentLinkPos * parentNode->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));

			currentNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - (parentRelative + currentRelative);
		}

		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGlobalPos, currentNodePtr->globalRotation);

		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);

		currentNodePtr->linkPtr = link;

		float jointSpace = 0.07f; // Gap Between joints

		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + 0.01f;

		PxCapsuleGeometry capsule(radius, capsuleHalfLength);

		link->createShape(capsule, *ragdollMaterial);
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f);

		if (currentNodePtr->parentNodeIDx != -1)
		{
			PxArticulationJoint* joint = link->getInboundJoint();

			PxQuat frameRotation = parentNode->globalRotation.getConjugate() * currentNodePtr->globalRotation;

			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentLinkPos * parentHalfLength, 0, 0), frameRotation);

			PxTransform thisConstraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos * childHalfLength, 0, 0));

			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}
		currentNode++;
	}
	return articulation;
}