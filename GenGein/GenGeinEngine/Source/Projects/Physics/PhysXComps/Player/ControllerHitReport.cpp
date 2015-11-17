#include <PxRigidDynamic.h>
#include <PxRigidActor.h>
#include "ControllerHitReport.h"


ControllerHitReport::~ControllerHitReport()
{
}

void ControllerHitReport::onShapeHit(const PxControllerShapeHit& a_hit)
{
	// Gets a reference to a structure which tells us what has been hit and where
	// Gets the actor from the shape we hit
	PxRigidActor* actor = a_hit.shape->getActor();

	// Gets the normal of the thing we hit and store it so the player controller
	// Can respond correctly
	m_playerContactNormal = a_hit.worldNormal;

	// Try to cast to a dynamic actor 
	PxRigidDynamic* hitActor = actor->isRigidDynamic();

	if (hitActor)
	{
		// this is where we can apply forces to things we hit
	}
}

void ControllerHitReport::onControllerHit(const PxControllersHit& a_hit)
{

}

void ControllerHitReport::onObstacleHit(const PxControllerObstacleHit& a_hit)
{

}