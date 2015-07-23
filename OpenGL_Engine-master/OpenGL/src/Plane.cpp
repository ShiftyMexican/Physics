#include "Plane.h"
#include <Gizmos.h>

Plane::Plane() : PhysicsObject()
{
	m_normal = glm::vec3(0, 1, 0);
	m_distance = -1;
	_shapeID = ShapeType::PLANE;
}

Plane::~Plane()
{
	
}

void Plane::Update(glm::vec3 gravity, float deltaTime)
{

}

void Plane::Draw()
{
	glm::vec3 centrePoint = m_normal * m_distance;
	glm::vec3 parallel = glm::vec3(m_normal.y, -m_normal.x, m_normal.z);
	glm::vec3 start = centrePoint + (parallel * 300.0f);
	glm::vec3 end = centrePoint - (parallel * 300.0f);

	Gizmos::addAABBFilled(centrePoint, glm::vec3(100, 0, 100), glm::vec4(0, 0, 1, 1));
}