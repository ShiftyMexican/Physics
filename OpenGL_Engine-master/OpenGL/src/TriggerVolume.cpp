#include "TriggerVolume.h"
#include "glm/ext.hpp"
#include "Gizmos.h"

TriggerVolume::TriggerVolume(PxScene* scene, PxRigidActor* actor) :
m_triggered(false),
m_actor(actor)
{
	m_actor->getShapes(&m_shape, 1);
	m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	//m_actor->attachShape(*m_shape);
	scene->setSimulationEventCallback(this);
}

TriggerVolume::~TriggerVolume()
{

}

void TriggerVolume::Update(float deltaTime)
{

}

void TriggerVolume::Draw()
{
	float width, height, length;
	PxBoxGeometry geometry;

	bool status = m_shape->getBoxGeometry(geometry);
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*m_shape, *m_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;

	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(0);

	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void TriggerVolume::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	m_triggered = true;
}

void TriggerVolume::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	m_triggered = true;
}