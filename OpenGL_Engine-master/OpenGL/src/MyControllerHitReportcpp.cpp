#include "MyControllerHitReport.h"

MyControllerHitReport::~MyControllerHitReport()
{

}

void MyControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	PxRigidActor* actor = hit.shape->getActor();

	_playerContactNormal = hit.worldNormal;


	if (actor)
	{
		PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();

	}
}