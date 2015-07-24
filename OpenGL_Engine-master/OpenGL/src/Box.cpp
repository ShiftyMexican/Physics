#include "Box.h"
#include <Gizmos.h>

Box::Box(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& extents, float mass, const glm::vec4& colour, float friction) :
RigidBody(position, velocity, m_rotation3D, mass, friction)
{
	m_colour = colour;
	m_extents = extents;
	_shapeID = ShapeType::BOX;
}

Box::~Box()
{

}

void Box::Draw()
{
	Gizmos::addAABB(m_position, m_extents, m_colour);
}