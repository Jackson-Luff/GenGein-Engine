#pragma once
#include <PhysX\PxPhysicsAPI.h>
#include <PhysX\PxScene.h>
#include <PhysX\pvd\PxVisualDebugger.h>
#include "Engine\Core\BaseApp.h"

using namespace physx;

class PhysX_testing : public BaseApp
{
public:
	PhysX_testing(c_int a_width, c_int a_height, c_pChar a_title);
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

	PxFoundation* m_pPhysicsFoundation;
	PxPhysics* m_pPhysics;
	PxScene* m_pPhysicsScene;
	PxMaterial* m_pPhysicsMaterial;
	PxMaterial* m_pBoxMaterial;
	PxCooking* m_pPhysicsCooker;
	
	PxDefaultErrorCallback m_defaultErrorCallBack;
	PxDefaultAllocator m_defaultAllocatorCallBack;
	PxSimulationFilterShader m_defaultFilterShader = PxDefaultSimulationFilterShader;
};