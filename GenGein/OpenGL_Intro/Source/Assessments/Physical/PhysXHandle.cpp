#include <glm/glm.hpp>

#include "Engine\Cameras\BaseCamera.h"
#include "Engine\MyAllocator.h"

#include "PhysXHandle.h"


PhysXHandle::PhysXHandle()
{}

PhysXHandle::~PhysXHandle()
{
	
}

void PhysXHandle::StartUp()
{
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
}

void PhysXHandle::Render()
{}

// Private Func's:

void PhysXHandle::SetUpEnvironment()
{
	// Add a plane
	PxTransform pose = PxTransform(PxVec3(0),
		PxQuat(PxHalfPi, PxVec3(0, 0, 1)));

	PxRigidStatic* plane = PxCreateStatic(*m_pPhysics,
		pose, PxPlaneGeometry(), *m_pPhysicsMaterial);

	// Add it to the PhysX scene
	m_pPhysicsScene->addActor(*plane);

	float density = 10.0f;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(0, 15, 0));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_pPhysics,
		transform, box, *m_pPhysicsMaterial, density);
	// Add it to the PhysX scene
	m_pPhysicsScene->addActor(*dynamicActor);
}

void PhysXHandle::SetUpPhysX()
{
	PxAllocatorCallback *myCallback = new MyAllocator();

	m_pPhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,
		*myCallback, m_defaultErrorCallBack);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION,
		*m_pPhysicsFoundation, PxTolerancesScale());

	PxInitExtensions(*m_pPhysics);

	// Create Physics Material
	m_pPhysicsMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -10.0f, 0.0f);
	sceneDesc.filterShader = &PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_pPhysicsScene = m_pPhysics->createScene(sceneDesc);

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
}

void PhysXHandle::SetUpVisualDebugger()
{
	// Check if PVDConnection manager is avaliabe on this platform
	if (m_pPhysics->getPvdConnectionManager() == NULL)
		return;

	// Setup connection parameters
	const char* pvd_host_ip = "127.0.0.1";
	// IP of the PC which is running PVD
	int port = 5425;
	// TCP port to connect tom where PVD is listening
	uint timeout = 100;
	// timeout in milliseconds to wait for PVD to respond,
	// consoles and remote PCs need a higher timeout.

	PxVisualDebuggerConnectionFlags connectionFlags =
		PxVisualDebuggerExt::getDefaultConnectionFlags();
	// and now try to connectPxVisualDebuggerExt
	auto theConnection = PxVisualDebuggerExt::createConnection(
		m_pPhysics->getPvdConnectionManager(),
		pvd_host_ip, port, timeout, connectionFlags);
}