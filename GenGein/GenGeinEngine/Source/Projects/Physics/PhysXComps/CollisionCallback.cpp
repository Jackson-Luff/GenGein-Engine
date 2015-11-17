#include <iostream>
#include "CollisionCallback.h"

void CollisionCallback::onContact(const PxContactPairHeader& a_pairHeader,
	const PxContactPair* a_pairs, PxU32 a_nbPairs)
{
	for (PxU32 i = 0; i < a_nbPairs; i++)
	{
		const PxContactPair& cp = a_pairs[i];
		// Only interested in touches found and lost
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			m_triggered = true;
			a_pairHeader.actors[0];
			a_pairHeader.actors[1];
		}
	}
};

void CollisionCallback::onTrigger(PxTriggerPair* a_pairs, PxU32 a_nbPairs)
{
	for (PxU32 i = 0; i < a_nbPairs; i++)
	{
		PxTriggerPair* tp = a_pairs + i;
		m_triggered = true;
		m_triggerBody = &tp->triggerActor;
	}
};

void CollisionCallback::onConstraintBreak(PxConstraintInfo*, PxU32)
{

};

void CollisionCallback::onWake(PxActor**, PxU32)
{

};

void CollisionCallback::onSleep(PxActor**, PxU32)
{

};
