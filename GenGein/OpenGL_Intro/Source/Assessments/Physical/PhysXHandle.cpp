#include <glm/glm.hpp>
#include <AIE\Gizmos.h>
#include "Engine\Cameras\BaseCamera.h"
#include "Engine\MyAllocator.h"
#include "Engine\Input\InputHandle.h"
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

void PhysXHandle::Update(const glm::mat4 a_CamWorld, const double a_dt)
{
	UpdatePhysX(a_dt);

	if (Input::Cursor::isLeftButtonDown())
	{
		int ran = rand() % 100;
		glm::vec3 pos = glm::vec3(a_CamWorld[3]);
		glm::vec3 dir = glm::vec3(a_CamWorld[2]);

		if (ran > 50)
			AddBox(pos, glm::vec3(0.5f), 10);
		else if (ran <= 50)
			AddSphere(pos, 1, 10);
	}
}

void PhysXHandle::Render(const glm::mat4& a_projView)
{
	Gizmos::clear();

	for (auto actor : m_physXActors)
	{
		PxU32 nShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape*[nShapes];
		actor->getShapes(shapes, nShapes);

		// Render all the shapes in the physX actor
		// ( for early tutorials there is just one )
		while (nShapes--)
		{
			RenderWidget(shapes[nShapes], actor);
		}
		delete[] shapes;
	}

	Gizmos::draw(a_projView);
}

void PhysXHandle::AddBox(glm::vec3 a_pos, glm::vec3 a_size, float a_density)
{
	PxBoxGeometry box(a_size.x, a_size.y, a_size.z);
	PxTransform transform(PxVec3(a_pos.x, a_pos.y, a_pos.z));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_pPhysics,
		transform, box, *m_pPhysicsMaterial, a_density);

	if (dynamicActor != NULL)
	{
		// Add it to the PhysX scene
		m_pPhysicsScene->addActor(*dynamicActor);
		m_physXActors.push_back(dynamicActor);
	}
}

void PhysXHandle::AddSphere(glm::vec3 a_pos, float a_radius, float a_density)
{
	PxSphereGeometry sphere(a_radius);
	PxTransform transform(PxVec3(a_pos.x, a_pos.y, a_pos.z));
	PxRigidDynamic* dynamicActor = PxCreateDynamic(*m_pPhysics,
		transform, sphere, *m_pPhysicsMaterial, a_density);

	if (dynamicActor != NULL)
	{
		m_pPhysicsScene->addActor(*dynamicActor);
		m_physXActors.push_back(dynamicActor);
	}
}

void PhysXHandle::AddPlane_Static(glm::vec3 a_pos, float a_radianOfRot, glm::vec3 a_axisRot)
{
	PxTransform pose = PxTransform(PxVec3(a_pos.x, a_pos.y, a_pos.z),
		PxQuat(a_radianOfRot, PxVec3(a_axisRot.x, a_axisRot.y, a_axisRot.z)));

	PxRigidStatic* plane = PxCreateStatic(*m_pPhysics,
		pose, PxPlaneGeometry(), *m_pPhysicsMaterial);
	// Add it to the PhysX scene
	if (plane != NULL)
	{
		m_pPhysicsScene->addActor(*plane);
	}
}

// Private Func's:

void PhysXHandle::RenderWidget(PxShape* a_shape, PxRigidActor* a_actor)
{
	PxGeometryType::Enum type = a_shape->getGeometryType();
	switch (type)
	{
		case PxGeometryType::eSPHERE:
			RenderGizmoSphere(a_shape, a_actor);
			break;
		case PxGeometryType::ePLANE:
			break;
		case PxGeometryType::eCAPSULE:
			break;
		case PxGeometryType::eBOX:
			RenderGizmoBox(a_shape, a_actor);
			break;
		case PxGeometryType::eCONVEXMESH:
			break;
		case PxGeometryType::eTRIANGLEMESH:
			break;
		case PxGeometryType::eHEIGHTFIELD:
			break;
		
	}
}

void PhysXHandle::RenderGizmoBox(PxShape* a_shape, PxRigidActor* a_actor)
{
	// Get the geometry for this PhysX collision volume
	PxBoxGeometry geo;
	float width = 1, height = 1, depth = 1;
	bool status = a_shape->getBoxGeometry(geo);

	if (status)
	{
		width = geo.halfExtents.x;
		height = geo.halfExtents.y;
		depth = geo.halfExtents.z;
	}

	// Get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*a_shape, *a_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	glm::vec3 position(m.getPosition().x, m.getPosition().y, m.getPosition().z);

	glm::vec3 extents = glm::vec3(width, height, depth);
	glm::vec4 colour = glm::vec4(1, 0, 0, 1);

	// Seriously horrid hack so i can show pickups a different colour
	if (a_actor->getName() != NULL && strcmp(a_actor->getName(), "Pickup1"))
	{
		colour = glm::vec4(0, 1, 0, 1);
	}
	//create our box gizmo 
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void PhysXHandle::RenderGizmoSphere(PxShape* a_shape, PxRigidActor* a_actor)
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
	PxMat44 m(PxShapeExt::getGlobalPose(*a_shape, *a_actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);

	glm::vec3 position(m.getPosition().x, m.getPosition().y, m.getPosition().z);

	glm::vec3 extents = glm::vec3(radius);
	glm::vec4 colour = glm::vec4(1, 0, 0, 1);

	// Seriously horrid hack so i can show pickups a different colour
	if (a_actor->getName() != NULL && strcmp(a_actor->getName(), "Pickup1"))
	{
		colour = glm::vec4(0, 1, 0, 1);
	}
	//create our box gizmo 
	Gizmos::addSphere(position, radius, 10, 10, colour, &M);
}

void PhysXHandle::SetUpEnvironment()
{
	// Add a plane
	AddPlane_Static(glm::vec3(0), 3.14159/2.0f, glm::vec3(0, 0, 1));

	// Add a cube
	AddBox(glm::vec3(0), glm::vec3(2), 10);

	// Add a cube
	//AddSphere(glm::vec3(0), 1.0f, 10);
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

