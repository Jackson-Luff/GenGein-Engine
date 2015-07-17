#pragma once
#include <vector>
#include <PhysX\PxPhysicsAPI.h>
#include <PhysX\PxScene.h>
#include <PhysX\pvd\PxVisualDebugger.h>

using namespace physx;

class PhysXHandle
{
public:
	PhysXHandle();
	~PhysXHandle();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const glm::mat4 a_CamWorld, const double a_dt);
	// Render things to screen
	virtual void Render(const glm::mat4& a_projView);

	virtual void AddBox(glm::vec3 a_pos, glm::vec3 a_size, float a_density);
	virtual void AddSphere(glm::vec3 a_pos, float a_radius, float a_density);
	virtual void AddPlane_Static(glm::vec3 a_pos, float a_radianOfRot, glm::vec3 a_axisRot);

private:

	void RenderWidget(PxShape* a_shape, PxRigidActor* a_actor);
	void RenderGizmoBox(PxShape* a_shape, PxRigidActor* a_actor);
	void RenderGizmoSphere(PxShape* a_shape, PxRigidActor* a_actor);

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

	std::vector<PxRigidActor*> m_physXActors;

};