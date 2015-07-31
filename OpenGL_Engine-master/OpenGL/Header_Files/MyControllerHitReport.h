/*
	Author: Jarrod Dowsey
	Description: Collision report class
*/

#ifndef	MY_CONTROLLER_HIT_REPORT_H
#define MY_CONTROLLER_HIT_REPORT_H

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>


using namespace physx;

class MyControllerHitReport : public PxUserControllerHitReport
{
public:

	MyControllerHitReport() : PxUserControllerHitReport(){};
	~MyControllerHitReport();

	virtual void onShapeHit(const PxControllerShapeHit &hit) override;

	virtual void onControllerHit(const PxControllersHit &hit){};

	virtual void onObstacleHit(const PxControllerObstacleHit &hit){};

	PxVec3 GetPlayerContactNormal() { return _playerContactNormal; };
	void ClearPlayerContactNormal(){ _playerContactNormal = PxVec3(0); };

	PxVec3 _playerContactNormal;

private:

};

#endif