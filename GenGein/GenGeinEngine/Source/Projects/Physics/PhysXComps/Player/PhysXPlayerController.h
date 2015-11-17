#pragma once
#include <PxPhysicsAPI.h>

using namespace physx;

class ControllerHitReport;

class PhysXPlayerController
{
public:
	PhysXPlayerController();
	~PhysXPlayerController();

	void SetUp(PxScene* a_scene, PxPhysics* a_physics, const PxExtendedVec3 a_position);
	void Shutdown();

	void Update(const PxVec3 a_gravity, const float a_dt);

	PxRigidDynamic* GetActor() const { return m_pPlayerActor; }

	PxVec3 GetPositon() const { 
		return m_pPlayerActor->getGlobalPose().p;
	}

private:

	PxRigidDynamic* m_pPlayerActor;
	PxMaterial* m_pPlayerMaterial;
	PxController* m_pPlayerController;
	PxControllerManager* m_pCharacterManager;

	ControllerHitReport* m_pHitReport;

	bool m_onGround;
	bool m_shouldJump;
	float m_moveSpeed;
	float m_rotSpeed;
	float m_jumpVel;
	PxVec3 m_velocity;
	PxVec3 m_position;
};