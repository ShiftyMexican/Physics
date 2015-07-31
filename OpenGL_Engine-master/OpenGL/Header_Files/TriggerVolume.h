/*
	Author: Jarrod Dowsey
	Description: This is the trigger volume class
*/

#ifndef TRIGGER_VOLUME_H
#define	TRIGGER_VOLUME_H

#include "PxPhysicsAPI.h"
using namespace physx;

class TriggerVolume : public PxSimulationEventCallback 
{
public:

	TriggerVolume(PxScene* scene, PxRigidActor* actor);
	~TriggerVolume();

	void Update(float deltaTime);
	void Draw();

	bool m_triggered;

private:

	PxShape* m_shape;
	PxRigidActor* m_actor;

	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor**, PxU32) {}
	virtual void onSleep(PxActor**, PxU32){}

};

#endif