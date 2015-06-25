#pragma once
#include <PhysX\PxPhysicsAPI.h>
#include <PhysX\PxScene.h>
#include <PhysX\pvd\PxVisualDebugger.h>
#include "Engine\Core\BaseApp.h"

class PhysX_testing : public BaseApp
{
public:
	PhysX_testing(const int32_t a_width, const int32_t a_height, const char* a_title);
	~PhysX_testing();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	void SetUpEnvironment();
	void SetUpPhysX();
	void UpdatePhysX(const double a_dt);
	void SetUpVisualDebugger();

	physx::PxFoundation* m_pPhysicsFoundation;
	physx::PxPhysics* m_pPhysics;
	physx::PxScene* m_pPhysicsScene;
	physx::PxMaterial* m_pPhysicsMaterial;
	physx::PxMaterial* m_pBoxMaterial;
	physx::PxCooking* m_pPhysicsCooker;
	
	physx::PxDefaultErrorCallback m_defaultErrorCallBack;
	physx::PxDefaultAllocator m_defaultAllocatorCallBack;
	physx::PxSimulationFilterShader m_defaultFilterShader = PxDefaultSimulationFilterShader;
};