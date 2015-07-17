/*
	Author: Jarrod Dowsey
	Description: Creating my own base physics object
*/

#ifndef	PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <glm/glm.hpp>

enum ShapeType
{
	PLANE = 0,
	SHPERE = 1,
	BOX = 2,
};

class PhysicsObject
{
public:
	ShapeType _shapeID;
	virtual void Update(glm::vec3 gravity, float timeStep) = 0;
	virtual void Draw() = 0;

private:

};

#endif