#include "Utilities.h"
#include <Gizmos.h>

Utilities::Utilities()
{

}

Utilities::~Utilities()
{

}

void Utilities::AddWidget(PxShape* _shape, PxRigidActor* _actor)
{
	PxGeometryType::Enum type = _shape->getGeometryType();

	switch (type)
	{
		case PxGeometryType::eBOX:
			AddBox(_shape, _actor);
			break;

		case PxGeometryType::eSPHERE:
			AddSphere(_shape, _actor);
			break;

		case PxGeometryType::eCAPSULE:
			AddCapsule(_shape, _actor);
			break;
	}
}

void Utilities::AddBox(PxShape* _pShape, PxRigidActor* _actor)
{
	// Get the Geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1, height = 1, length = 1;
	bool status = _pShape->getBoxGeometry(geometry);

	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}

	// Get the Transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*_pShape, *_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	glm::vec3 position;

	// Get the position out of the Transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(1, 0, 0, 1);

	if (_actor->getName() != NULL && strcmp(_actor->getName(), "Pickup1"))
	{
		colour = glm::vec4(0, 1, 0, 1);
	}

	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void Utilities::AddSphere(PxShape* _pShape, PxRigidActor* _actor)
{
	PxSphereGeometry geometry;
	float radius = 2.0f;

	bool status = _pShape->getSphereGeometry(geometry);

	if (status)
	{
		radius = geometry.radius;
	}

	// Get the Transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*_pShape, *_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	glm::vec3 position;

	// Get the position out of the Transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec4 colour = glm::vec4(0, 1, 0, 1);

	Gizmos::addSphere(position, radius, 10, 10, colour, &M);
}

void Utilities::AddCapsule(PxShape* _pShape, PxRigidActor* _actor)
{
	PxCapsuleGeometry geometry;
	float radius = 0.2f;
	float halfHeight;

	bool status = _pShape->getCapsuleGeometry(geometry);

	if (status)
	{
		radius = geometry.radius;
		halfHeight = geometry.halfHeight;
	}

	PxTransform transform = PxShapeExt::getGlobalPose(*_pShape, *_actor);

	// Get the Transform for this PhysX collision volume
	PxMat44 m(transform);

	glm::mat4* M = (glm::mat4*)(&m); 

	// Get the position out of the Transform
	glm::vec3 position;
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;
	
	glm::vec4 axis(halfHeight, 0, 0, 0);
	axis = *M * axis;

	glm::vec4 colour = glm::vec4(0.8, 0, 1, 1);
	glm::mat4 m2 = glm::rotate(*M, 11 / 7.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	Gizmos::addSphere(position + axis.xyz(), radius, 10, 10, colour);
	Gizmos::addSphere(position - axis.xyz(), radius, 10, 10, colour);
	Gizmos::addCylinderFilled(position, radius, halfHeight, 10, colour, &m2);
}
