#pragma once
#include <PxPhysics.h>
#include <characterkinematic\PxController.h>

using namespace physx;

class ControllerHitReport : public PxUserControllerHitReport
{
public:
	//ControllerHitReport();
	~ControllerHitReport();

	// Called when current controller hits a user-defined obstacle
	ControllerHitReport() 
		: PxUserControllerHitReport() 
	{
		m_playerContactNormal = PxVec3(0);
	};

	// Overload the onShapeHit function
	virtual void onShapeHit(const PxControllerShapeHit& a_hit);

	// Other collision functions which we must overload
	// These handle collision with other controllers and hitting obstacles
	virtual void onControllerHit(const PxControllersHit& a_hit);
	// Called when current controller hits another controller.. and more
	virtual void onObstacleHit(const PxControllerObstacleHit& a_hit);

	// Get contact normal
	inline const PxVec3& GetPlayerContactNormal() 
		{ return m_playerContactNormal; }

	// Clear contact normal
	inline void ClearPlayerContactNormal()
		{ m_playerContactNormal = PxVec3(0); }

private:
	PxVec3 m_playerContactNormal;
};