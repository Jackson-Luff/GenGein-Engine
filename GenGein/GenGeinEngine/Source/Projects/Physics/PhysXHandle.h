#pragma once
#include <vector>

#include <PxPhysicsAPI.h>
#include <pvd\PxVisualDebugger.h>

#include "PhysXComps\PhysXRagdoll.h"

using namespace physx;

class FBXModel;
class ParticleFluidEmitter;
class PhysXPlayerController;
class CollisionCallback;

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
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();
private:

	struct FilterGroup
	{
		enum Enum
		{
			ePLAYER = (1 << 0),
			ePLATFORM = (1 << 1),
			eGROUND = (1 << 2),
			eRAGDOLL = (1 << 3)
		};
	};

	enum PhysXActorType
	{
		STATIC,
		DYNAMIC
	};

	struct PhysXRigidActor
	{
	public:
		// Sphere
		PhysXRigidActor(PxPhysics* a_physics, PxTransform a_pose, const float a_radius, PhysXActorType a_objType, PxMaterial* a_material, const float a_density = 1)
			: PhysXRigidActor(a_physics, a_pose, &PxSphereGeometry(a_radius), a_objType, a_material, a_density)
		{}
		// Plane
		PhysXRigidActor(PxPhysics* a_physics, PxTransform a_pose, PhysXActorType a_objType, PxMaterial* a_material)
			: PhysXRigidActor(a_physics, a_pose, &PxPlaneGeometry(), a_objType, a_material)
		{}
		// Box
		PhysXRigidActor(PxPhysics* a_physics, PxTransform a_pose, const PxVec3 a_size, PhysXActorType a_objType, PxMaterial* a_material, const float a_density = 1)
			: PhysXRigidActor(a_physics, a_pose, &PxBoxGeometry(a_size.x, a_size.y, a_size.z), a_objType, a_material, a_density)
		{}

		PxRigidActor* GetActor() { return actor; }
		void SetActor(PxRigidActor* a_actor) { actor = a_actor; }

	private:
		PhysXRigidActor(PxPhysics* a_physics, PxTransform a_pose, PxGeometry* a_geo, PhysXActorType a_objType, PxMaterial* a_material, const float a_density = NULL)
		{
			switch (a_objType)
			{
			case PhysXActorType::STATIC:
				actor = PxCreateStatic(*a_physics, a_pose, *a_geo, *a_material);
				break;
			case PhysXActorType::DYNAMIC:
				actor = PxCreateDynamic(*a_physics, a_pose, *a_geo, *a_material, a_density);
				break;
			default:
				break;
			}

			if (actor == NULL)
			{
				printf("WARNING: Actor is null\n");
			}
		}
	
		PxRigidActor* actor;
	};

	PxArticulation* MakeRagDoll(PxPhysics* a_physics, RagDoll::RagDollNode** a_nodeArray,
		PxTransform a_worldPos, float a_scaleFactor, PxMaterial* a_ragdollMaterial);

	PxRigidActor* AddBox(const PxTransform a_pose, const PxVec3 a_size, const float a_density, PhysXActorType a_type, const bool a_trigger = false);
	PxRigidActor* AddSphere(const PxTransform a_pose, const float a_radius, const float a_density, PhysXActorType a_type, const bool a_trigger = false);
	PxRigidActor* AddPlane(const PxTransform a_pose, PhysXActorType a_type);
	void AddRagDoll(const PxTransform a_pose);
	void AddFluidSimWithContainer(const PxVec3 a_position);

	void RenderWidget(PxShape* a_shape, PxRigidActor& a_actor);
	void RenderPlane(PxShape* a_shape, PxRigidActor& a_actor);
	void RenderSphere(PxShape* a_shape, PxRigidActor& a_actor);
	void RenderBox(PxShape* a_shape, PxRigidActor& a_actor);
	void RenderCapsule(PxShape* a_shape, PxRigidActor& a_actor);
	
	void SetupFilter(PxRigidActor* a_actor, PxU32 a_filterGroup, PxU32 a_filterMask);
	void SetUpPhysX();
	void BuildStaticBarriers();
	void BuildFluidSim();
	void BuildTower();
	void BuildRagdollSkit();
	void SetUpEnvironment();
	void UpdatePhysX(const double a_dt);
	void SetUpVisualDebugger();

	CollisionCallback* m_pCollisionCallback;
	ParticleFluidEmitter* m_pParticleEmitter;
	PxFoundation* m_pPhysicsFoundation;
	PxPhysics* m_pPhysics;
	PxScene* m_pPhysicsScene;
	PxMaterial* m_pPhysicsMaterial;
	PxMaterial* m_pBoxMaterial;
	PxCooking* m_pPhysicsCooker;

	PxDefaultErrorCallback m_defaultErrorCallBack;
	PxDefaultAllocator m_defaultAllocatorCallBack;

	std::vector<PxRigidActor*> m_physXActors;
	std::vector<PxArticulation*> m_physXRagDollActors;

	PhysXPlayerController* m_player;

	// Instances of re-used models:
	FBXModel* m_fluidInst;
	FBXModel* m_sphereInst;
	FBXModel* m_planeInst;
	FBXModel* m_boxInst;
	FBXModel* m_capsuleInst;
};