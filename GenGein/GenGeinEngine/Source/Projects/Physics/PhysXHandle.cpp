#include <glm/glm.hpp>
#include <AIE\Gizmos.h>

#include "Engine\Objects\FBXModel.h"
#include "Engine\Cameras\BaseCamera.h"
#include "Engine\Input\InputHandle.h"
#include "PhysXComps\MemAlloc.h"
#include "PhysXComps\PhysXRagdoll.h"
#include "PhysXComps\Particles\ParticleFluidEmitter.h"
#include "PhysXComps\Player\PhysXPlayerController.h"
#include "PhysXComps\CollisionCallback.h"
#include "PhysXHandle.h"

using namespace RagDoll;

PhysXHandle::PhysXHandle()
{}

PhysXHandle::~PhysXHandle()
{
	m_planeInst->Shutdown();
	m_planeInst = NULL;
	m_sphereInst->Shutdown();
	m_sphereInst = NULL;
	m_boxInst->Shutdown();
	m_boxInst = NULL;
	m_player->Shutdown();
	m_player = NULL;

	//for (unsigned int i = 0; i < m_physXActors.size(); i++)
	//{
	//	delete m_physXActors[i];
	//	i--;
	//}
}

void PhysXHandle::StartUp()
{
	m_sphereInst = new FBXModel();
	m_sphereInst->LoadFBX("PhysX", "Data/Models/sphere.fbx", FBXFile::UNITS_CENTIMETER);

	m_planeInst = new FBXModel();
	m_planeInst->LoadFBX("PhysX", "Data/Models/plane.fbx", FBXFile::UNITS_CENTIMETER);

	m_boxInst = new FBXModel();
	m_boxInst->LoadFBX("PhysX", "Data/Models/cube.fbx", FBXFile::UNITS_CENTIMETER);

	m_capsuleInst = new FBXModel();
	m_capsuleInst->LoadFBX("PhysX", "Data/Models/capsule.fbx", FBXFile::UNITS_CENTIMETER);

	m_fluidInst = new FBXModel();
	m_fluidInst->LoadFBX("Fluid", "Data/Models/sphere.fbx", FBXFile::UNITS_CENTIMETER);

	m_pParticleEmitter = nullptr;
	m_player = nullptr;
	m_pCollisionCallback = new CollisionCallback();

	SetUpPhysX();
	SetUpVisualDebugger();
	SetUpEnvironment();
}

void PhysXHandle::ShutDown()
{
	m_pPhysicsScene->release();
	m_pPhysics->release();
	m_pPhysicsFoundation->release();
}

void PhysXHandle::Update(const double a_dt)
{
	UpdatePhysX(a_dt);

	if (Input::Cursor::isLeftButtonDown())
	{
		glm::mat4 world = Input::Camera::GetCameraWorldTrans();

		int ran = rand() % 100;
		glm::vec3 pos = glm::vec3(world[3]);
		glm::vec3 dir = -glm::vec3(world[2]);

		PxTransform pose(PxVec3(pos.x, pos.y, pos.z + dir.z * 5.0f));
		PhysXRigidActor* sphereActor = new PhysXRigidActor(m_pPhysics, pose, 1.0f, DYNAMIC, m_pPhysicsMaterial, 1.0f);
		PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(sphereActor->GetActor());
		actor->setLinearVelocity(PxVec3(dir.x, dir.y, dir.z) * 100.0f);
		m_physXActors.push_back(sphereActor->GetActor());
		m_pPhysicsScene->addActor(*sphereActor->GetActor());
	}

	if (m_pParticleEmitter)
	{
		m_pParticleEmitter->Update(a_dt*2.0f);
	}

	if (m_player)
	{
		m_player->Update(m_pPhysicsScene->getGravity(), (float)a_dt);

		PxVec3 pos = m_player->GetPositon();
		Input::Camera::SetCamPos(glm::vec3(pos.x, pos.y, pos.z));
	}

	if (m_pCollisionCallback)
	{
		if (m_pCollisionCallback->GetTriggered())
		{
			0;
			//PxRigidActor* dynamicActor = static_cast<PxRigidActor*>( (*(m_pCollisionCallback->GetTriggerBody()))->userData);
			//
			//*m_pCollisionCallback->GetTriggerBody() = dynamicActor;
		}
	}
}

void PhysXHandle::Render()
{
	// Render list of physics actors
	for (auto& a : m_physXActors)
	{
		PxRigidActor& actor = *a;
		PxU32 nShapes = actor.getNbShapes();
		PxShape** shapes = new PxShape*[nShapes];
		actor.getShapes(shapes, nShapes);

		// Render all the shapes in the physX actor
		// ( for early tutorials there is just one )
		while (nShapes--)
		{
			RenderWidget(shapes[nShapes], *a);
		}
		delete[] shapes;
	}

	// Render list of ragdoll actors
	for (auto& articulators : m_physXRagDollActors)
	{
		PxU32 nLinks = articulators->getNbLinks();
		PxArticulationLink** links = new PxArticulationLink*[nLinks];
		articulators->getLinks(links, nLinks);
		//if (1)
		{
			while (nLinks--)
			{
				PxArticulationLink* link = links[nLinks];
				PxU32 nShapes = link->getNbShapes();
				PxShape** shapes = new PxShape*[nShapes];
				link->getShapes(shapes, nShapes);
				while (nShapes--)
				{
					RenderWidget(shapes[nShapes], *link);
				}
			}
		}
	}

	if (m_pParticleEmitter)
	{
		m_pParticleEmitter->RenderParticles();
	}
}

void SetActorAsTrigger(PxRigidActor* a_actor)
{
	PxRigidStatic* staticActor = a_actor->isRigidStatic();
	assert(staticActor);

	const PxU32 numShapes = staticActor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)* numShapes, 16);
	staticActor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

PxRigidActor* PhysXHandle::AddBox(const PxTransform a_pose, const PxVec3 a_size, const float a_density, PhysXActorType a_type, const bool a_trigger)
{
	// Add it to the render list
	PhysXRigidActor* boxActor = new PhysXRigidActor(m_pPhysics, a_pose, a_size, a_type, m_pPhysicsMaterial, a_density);

	//if (a_trigger && boxActor != NULL)
	//{
	//	SetActorAsTrigger(boxActor->GetActor());
	//	SetupFilter(boxActor->GetActor(), FilterGroup::ePLATFORM, FilterGroup::ePLAYER);
	//}

	m_physXActors.push_back(boxActor->GetActor());
	// Add it to the PhysX scene
	m_pPhysicsScene->addActor(*boxActor->GetActor());

	return boxActor->GetActor();
}

PxRigidActor* PhysXHandle::AddSphere(const PxTransform a_pose, const float a_radius, const float a_density, PhysXActorType a_type, const bool a_trigger)
{
	PhysXRigidActor* sphereActor = new PhysXRigidActor(m_pPhysics, a_pose, a_radius, a_type, m_pPhysicsMaterial, a_density);
	
	if (a_trigger)
		SetActorAsTrigger(sphereActor->GetActor());

	m_physXActors.push_back(sphereActor->GetActor());
	m_pPhysicsScene->addActor(*sphereActor->GetActor());
	return sphereActor->GetActor();
}

PxRigidActor* PhysXHandle::AddPlane(const PxTransform a_pose, PhysXActorType a_type)
{
	// Add it to the render list
	PhysXRigidActor* planeActor = new PhysXRigidActor(m_pPhysics, a_pose, a_type, m_pPhysicsMaterial);
	//SetupFilter(planeActor->GetActor(), FilterGroup::ePLAYER, FilterGroup::eGROUND);

	m_physXActors.push_back(planeActor->GetActor());
	// Add it to the PhysX scene
	m_pPhysicsScene->addActor(*planeActor->GetActor());

	return planeActor->GetActor();
}

void PhysXHandle::AddRagDoll(const PxTransform a_pose)
{
	const PxVec3 X_AXIS = PxVec3(1, 0, 0);
	const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
	const PxVec3 Z_AXIS = PxVec3(0, 0, 1);

	//complex humanoid ragdoll example
	RagDoll::RagDollNode* ragdollData[] =
	{
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT,				1, 3, 1, 1, "Lower spine"),
		new RagDoll::RagDollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE,						1, 1, -1, 1, "Left pelvis"),
		new RagDoll::RagDollNode(PxQuat(0.0f, Z_AXIS), LOWER_SPINE,						1, 1, -1, 1, "Right pelvis"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_PELVIS,		5, 2, -1, 1, "L upper leg"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_PELVIS,		5, 2, -1, 1, "R upper leg"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_UPPER_LEG,	5, 1.75f, -1, 1, "L lower leg"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_UPPER_LEG,	5, 1.75f, -1, 1, "R lowerleg"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE,				1, 3, 1, -1, "Upper spine"),
		new RagDoll::RagDollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE,						1, 1.5, 1, 1, "Left clavicle"),
		new RagDoll::RagDollNode(PxQuat(0.0f, Z_AXIS), UPPER_SPINE,						1, 1.5, 1, 1, "Right clavicle"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE,				1, 1, 1, -1, "Neck"),
		new RagDoll::RagDollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK,						1, 3, 1, -1, "Head"),
		new RagDoll::RagDollNode(PxQuat(PxPi - 0.3f, Z_AXIS), LEFT_CLAVICLE,			3, 1.5f, -1, 1, "Left upper arm"),
		new RagDoll::RagDollNode(PxQuat(0.3f, Z_AXIS), RIGHT_CLAVICLE,					3, 1.5f, -1, 1, "Right upper arm"),
		new RagDoll::RagDollNode(PxQuat(PxPi - 0.3f, Z_AXIS), LEFT_UPPER_ARM,			3, 1, -1, 1, "Left lower arm"),
		new RagDoll::RagDollNode(PxQuat(0.3f, Z_AXIS), RIGHT_UPPER_ARM,					3, 1, -1, 1, "Right lower arm"),
		NULL
	};

	PxArticulation* ragDollArticulation;
	ragDollArticulation = MakeRagDoll(m_pPhysics, ragdollData,
		a_pose, 0.1f, m_pPhysicsMaterial);
	
	m_pPhysicsScene->addArticulation(*ragDollArticulation);
	m_physXRagDollActors.push_back(ragDollArticulation);
}

void PhysXHandle::AddFluidSimWithContainer(const PxVec3 a_position)
{
#pragma region CreatingStaticEnvironment

	// Setting up the plane platform
	PhysXRigidActor* box;
	
	// Size
	PxTransform pose;
	PxVec3 side1(9, 2, 1);
	PxVec3 side2(1, 2, 9);

	for (int i = 0; i < 3; i++)
	{
		// Positioning
		if (i == 0)
			pose = PxTransform(PxVec3(+0, 1, -8) + a_position, PxQuat(3.14159f / 24.0f, PxVec3(0, 0, 1)));
		else if (i == 1)			   							
			pose = PxTransform(PxVec3(+8, 1, +0) + a_position, PxQuat(3.14159f / 24.0f, PxVec3(0, 0, 1)));
		else if (i == 2)			  							
			pose = PxTransform(PxVec3(-0, 1, +8) + a_position, PxQuat(3.14159f / 24.0f, PxVec3(0, 0, 1)));

		if (i == 1)
		{ 
			box = new PhysXRigidActor(m_pPhysics, pose, side2, STATIC, m_pPhysicsMaterial);
			m_pPhysicsScene->addActor(*box->GetActor());
			m_physXActors.push_back(box->GetActor());
		}
		else
		{
			box = new PhysXRigidActor(m_pPhysics, pose, side1, STATIC, m_pPhysicsMaterial);
			m_pPhysicsScene->addActor(*box->GetActor());
			m_physXActors.push_back(box->GetActor());
		}	
	}
#pragma endregion

	PxParticleFluid* pf;

	// Create particle system in PhysX
	// Set immutable properties
	PxU32 maxParticles = 4000;
	bool perParticleRestOffset = false;
	pf = m_pPhysics->createParticleFluid(maxParticles, perParticleRestOffset);

	pf->setRestParticleDistance(0.5f);
	pf->setDynamicFriction(0.01f);
	pf->setStaticFriction(0.01f);
	pf->setDamping(0.2f);
	pf->setParticleMass(0.1f);
	pf->setRestitution(0.0f);
	pf->setViscosity(1.0f);
	pf->setGridSize(10);
	pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	pf->setStiffness(50);

	// Once we have a valid particle system, create an emitter
	if (pf)
	{
		m_pPhysicsScene->addActor(*pf);
	
		m_pParticleEmitter = new ParticleFluidEmitter(maxParticles,
			a_position + PxVec3(0, 3, 1), pf, 0.025f, m_fluidInst);

		m_pParticleEmitter->SetStartVelocityRange(
			-0.001f, -350.0f, -0.001f, 0.001f, -350.0f, -0.001f);
	}
}

// Private Func's:

PxArticulation* PhysXHandle::MakeRagDoll(PxPhysics* a_physics, RagDoll::RagDollNode** a_nodeArray,
	PxTransform a_worldPos, float a_scaleFactor, PxMaterial* a_ragdollMaterial)
{
	// Create the articulation for our ragdoll
	PxArticulation* articulation = a_physics->createArticulation();

	RagDoll::RagDollNode** currNode = a_nodeArray;

	// While there are more nodes to process
	while (*currNode != NULL)
	{
		// Get a pointer to the current node
		RagDoll::RagDollNode* currNodePtr = *currNode;
		// Create a pointer ready to hold the parent
		// node pointer if there is one
		RagDoll::RagDollNode* parentNode = nullptr;
		// Get scaled values for capsule
		float radius = currNodePtr->radius * a_scaleFactor;
		float halfLength = currNodePtr->halfLength * a_scaleFactor;
		float chldHalfLngth = radius + halfLength;
		float parentHalfLngth = 0;

		PxArticulationLink* parentLinkPtr = NULL;
		currNodePtr->scaledGlobalPos = a_worldPos.p;

		// Check for parent
		if (currNodePtr->parentNodeIdx != -1)
		{
			// Parent found!

			// Get a pointer to the parent node
			parentNode = *(a_nodeArray + currNodePtr->parentNodeIdx);
			// Get a pointer to the link for the parent
			parentLinkPtr = parentNode->linkPtr;
			parentHalfLngth = (parentNode->radius + parentNode->halfLength) * a_scaleFactor;

			// Work out the local position of the node
			PxVec3 currRel = currNodePtr->childLinkPos * currNodePtr->globalRot.rotate(PxVec3(chldHalfLngth, 0, 0));
			PxVec3 parentRel = -currNodePtr->parentLinkPos * parentNode->globalRot.rotate(PxVec3(parentHalfLngth, 0, 0));
			currNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - (parentRel + currRel);
		}
		/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ///

		// Build a transform for the link
		PxTransform linkTrans = PxTransform(currNodePtr->scaledGlobalPos, currNodePtr->globalRot);
		// Create a link in the articulation
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTrans);
		// Add the pointer to this link into the ragdoll 
		// data so we have it for later when we want to link it
		currNodePtr->linkPtr = link;

		float jointSpace = 0.01f;
		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + 0.01f;
		PxCapsuleGeometry capsule(radius, capsuleHalfLength);

		// Adds a capsule collider to the link
		link->createShape(capsule, *a_ragdollMaterial);
		// Adds some mass, mash should really be part of data!
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f);

		/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ///

		// Check for parent
		if (currNodePtr->parentNodeIdx != -1)
		{
			// Get the pointer to the joint from the link
			PxArticulationJoint* joint = link->getInboundJoint();
			// Get the relative rotation of this link
			PxQuat frameRot = parentNode->globalRot.getConjugate() * currNodePtr->globalRot;
			// Set the parent constraint frame
			PxTransform parentConstraintFrame = PxTransform(PxVec3(currNodePtr->parentLinkPos *
				parentHalfLngth, 0, 0), frameRot);

			// Set the child constraint frame (this the constraint
			// frame of the newly added link)
			PxTransform thisConstraintFrame = PxTransform(PxVec3(currNodePtr->childLinkPos *
				chldHalfLngth, 0, 0));

			// Set up the poses for the join so it is in the correct place
			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);

			// Set up some constraints to stop it flopping around
			joint->setStiffness(20.0f);
			joint->setDamping(20.0f);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}

		currNode++;
	}

	return articulation;
}

void PhysXHandle::RenderWidget(PxShape* a_shape, PxRigidActor& a_actor)
{
	PxGeometryType::Enum type = a_shape->getGeometryType();
	switch (type)
	{
		case PxGeometryType::eSPHERE:
			RenderSphere(a_shape, a_actor);
			break;
		case PxGeometryType::ePLANE:
			RenderPlane(a_shape, a_actor);
			break;
		case PxGeometryType::eCAPSULE:
			RenderCapsule(a_shape, a_actor);
			break;
		case PxGeometryType::eBOX:
			RenderBox(a_shape, a_actor);
			break;
		case PxGeometryType::eCONVEXMESH:
			break;
		case PxGeometryType::eTRIANGLEMESH:
			break;
		case PxGeometryType::eHEIGHTFIELD:
			break;
	}
}

void PhysXHandle::RenderPlane(PxShape* a_shape, PxRigidActor& a_actor)
{
	glm::vec3 p(55.0f, 0, 0);
	glm::vec3 s(2.5f, 1, 2.5f);
	m_planeInst->SetLocalTransform(glm::translate(p) * glm::scale(s));
	m_planeInst->Render();
}

void PhysXHandle::RenderSphere(PxShape* a_shape, PxRigidActor& a_actor)
{
	// Get the geometry for this PhysX collision volume
	PxSphereGeometry geo;
	float radius = 0.5f;
	bool status = a_shape->getSphereGeometry(geo);

	if (status)
	{
		radius = geo.radius;
	}

	// Get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*a_shape, a_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	m_sphereInst->SetLocalTransform(M);
	m_sphereInst->Render();
}

void PhysXHandle::RenderBox(PxShape* a_shape, PxRigidActor& a_actor)
{
	// Get the geometry for this PhysX collision volume
	PxBoxGeometry geo;
	glm::vec3 size;
	bool status = a_shape->getBoxGeometry(geo);

	if (status)
	{
		size.x = geo.halfExtents.x;
		size.y = geo.halfExtents.y;
		size.z = geo.halfExtents.z;
	}

	// Get the transform for this PhysX collision volume
	PxMat44 m(a_actor.getGlobalPose());
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	m_boxInst->SetLocalTransform(M * glm::scale(size));
	m_boxInst->Render();
}

void PhysXHandle::RenderCapsule(PxShape* a_shape, PxRigidActor& a_actor)
{
	// Get the geometry for this PhysX collision volume
	PxCapsuleGeometry geo;
	float halfHeight = 1.0f, radius = 1.0f;
	bool status = a_shape->getCapsuleGeometry(geo);

	if (status)
	{
		halfHeight = geo.halfHeight;
		radius = geo.radius;
	}

	// Get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*a_shape, a_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	// FIX
	m_capsuleInst->SetLocalTransform(M);
	m_capsuleInst->Render();
}

PxFilterFlags FilterShader(PxFilterObjectAttributes a_a0, PxFilterData a_fd0,
	PxFilterObjectAttributes a_a1, PxFilterData a_fd1,
	PxPairFlags& a_pairFlags, const void* a_conBlock, PxU32 a_conBlockSize)
{
	// Let triggers through
	if (PxFilterObjectIsTrigger(a_a0) || PxFilterObjectIsTrigger(a_a1))
	{
		a_pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// Generate contacts for all that were not filtered above
	a_pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// Trigger the contact callback for pairs(A,B) where
	// the filtermask of A contains the Id of B and vice versa
	if ((a_fd0.word0 & a_fd1.word0) && (a_fd1.word0 & a_fd0.word0))
		a_pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

	return PxFilterFlag::eDEFAULT;
}

void PhysXHandle::SetupFilter(PxRigidActor* a_actor, PxU32 a_filterGroup, PxU32 a_filterMask)
{
	PxFilterData fData;
	fData.word0 = a_filterGroup; // word0 = ID
	fData.word1 = a_filterMask;  // word1 = ID make to filter pairs
	const PxU32 numShapes = a_actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)* numShapes, 16);

	a_actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(fData);
	}

	_aligned_free(shapes);
}

void PhysXHandle::SetUpPhysX()
{
	PxAllocatorCallback *myCallback = new MemAlloc();

	m_pPhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,
		*myCallback, m_defaultErrorCallBack);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION,
		*m_pPhysicsFoundation, PxTolerancesScale());
	
	PxInitExtensions(*m_pPhysics);
	// Create Physics Material
	m_pPhysicsMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.87f, 0.0f);
	sceneDesc.filterShader = FilterShader;
	sceneDesc.simulationEventCallback = m_pCollisionCallback;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_pPhysicsScene = m_pPhysics->createScene(sceneDesc);
}

void PhysXHandle::BuildStaticBarriers()
{
	// Right
	AddBox(PxTransform(PxVec3(110, 10.0f, 0.0f)), PxVec3(0.5f, 20.0f, 55), 6.0f, STATIC);
	// Back 
	AddBox(PxTransform(PxVec3(55, 10.0f, -55)), PxVec3(55, 20.0f, 0.5f), 6.0f, STATIC);
	// Front Fence
	AddBox(PxTransform(PxVec3(55, 2.0f, 55)), PxVec3(55, 3.0f, 0.5f), 6.0f, STATIC);

	// Add a plane
	AddPlane(PxTransform(PxVec3(0),PxQuat(3.14159f / 2.0f, PxVec3(0,0,1))), STATIC);
}

void PhysXHandle::BuildFluidSim()
{
	AddFluidSimWithContainer(PxVec3(100.0f, 11.0f, 30.0f));
	AddBox(PxTransform(PxVec3(100.0f, 5.5f, 30.0f), PxQuat(3.14159f / 24.0f, PxVec3(0, 0, 1))), PxVec3(10, 1.0f, 10), 11.0f, PhysXActorType::STATIC);
	AddBox(PxTransform(PxVec3(82.5f, 2.25f, 30.0f), PxQuat(3.14159f/14.0f, PxVec3(0,0,1))), PxVec3(10, 1.0f, 10), 11.0f, PhysXActorType::STATIC);
}

void PhysXHandle::BuildTower()
{
	int i = -1, c = 20;
	for (int j = 0; j < c; j++)
	{
		for (i = c - j; i > 0; i--)
		{
			PxVec3 p(30.0f, 0.0f, -30.0f);
			AddBox(PxTransform(p + PxVec3((i * 2.0f) + j, 1.0f + (1.5f * j), 0.0f)), PxVec3(1.0f, 0.75f, 0.75f), 5.0f, PhysXActorType::DYNAMIC);
		}
	}
}

void PhysXHandle::BuildRagdollSkit()
{
	int boxCount = 8;
	PxVec3 pos = PxVec3(20, 40, 20);
	AddRagDoll(PxTransform(PxVec3(pos.x + boxCount, pos.y, pos.z + boxCount)));

	for (int i = 0; i < boxCount; i++)
	{
		for (int j = 0; j < boxCount; j++)
		{
			PxRigidActor* actor = AddBox(PxTransform(pos + PxVec3(i*2.0f, -20.0f, j*2.0f)), PxVec3(1.0f), 10.0f, PhysXActorType::DYNAMIC, true);
		}
	}
}

void PhysXHandle::SetUpEnvironment()
{
	BuildStaticBarriers();
	BuildFluidSim();
	BuildTower();	
	BuildRagdollSkit();

	m_player = new PhysXPlayerController();
	m_player->SetUp(m_pPhysicsScene, m_pPhysics, PxExtendedVec3(60, 17.5f, 50));
	SetupFilter(m_player->GetActor(), FilterGroup::ePLAYER, FilterGroup::ePLATFORM);
}

void PhysXHandle::UpdatePhysX(const double a_dt)
{
	if (a_dt <= 0)
		return;

	m_pPhysicsScene->simulate((float)a_dt);

	while (!m_pPhysicsScene->fetchResults())
	{
		// don't need to do anything here yet,
		// but we have to fetch results.
	}

	// Add widgets to represent all the physX actors which are in the scene
	for (auto& a : m_physXActors)
	{
		PxRigidActor& actor = *a;
		if (a->userData != nullptr)
		{
			PxMat44 m = actor.getGlobalPose();
			FBXModel* meshComp = (FBXModel*)actor.userData;
			glm::mat4 M = *((glm::mat4*)(&m)); // Ugly cast from Px to GLM
			meshComp->SetLocalTransform(M);
		}
	}
}

void PhysXHandle::SetUpVisualDebugger()
{
	// Check if PVDConnection manager is avaliabe on this platform
	if (m_pPhysics->getPvdConnectionManager() == NULL)
	{
		printf("Warning: PhysX Visual Debugger not found running!\n");
		return;
	}

	// Setup connection parameters
	const char* pvd_host_ip = "127.0.0.1";
	// IP of the PC which is running PVD
	int port = 5425;
	// TCP port to connect tom where PVD is listening
	unsigned int timeout = 100;
	// timeout in milliseconds to wait for PVD to respond,
	// consoles and remote PCs need a higher timeout.

	PxVisualDebuggerConnectionFlags connectionFlags =
		PxVisualDebuggerExt::getDefaultConnectionFlags();

	// and now try to connectPxVisualDebuggerExt
	physx::debugger::comm::PvdConnection* conn = PxVisualDebuggerExt::createConnection(
		m_pPhysics->getPvdConnectionManager(),
		pvd_host_ip, port, timeout, connectionFlags);

	if (conn)
	{
		printf("Connected to PhysX Visual Debugger!\n");
		m_pPhysics->getVisualDebugger()->setVisualizeConstraints(true);
	}
}
