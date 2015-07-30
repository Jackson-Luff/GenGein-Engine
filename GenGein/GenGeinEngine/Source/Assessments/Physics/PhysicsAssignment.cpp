#include "Engine\Core\ShaderHandler.h"
#include "Engine\Cameras\BaseCamera.h"
#include "PhysHandle.h"
#include "PhysXHandle.h"

#include "PhysicsAssignment.h"

#define PHYSTEST 0

PhysicsAssignment::PhysicsAssignment() : BaseApp()
{}

PhysicsAssignment::PhysicsAssignment(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

PhysicsAssignment::~PhysicsAssignment()
{
	delete m_physicsHandle;
	delete m_physXHandle;
}

void PhysicsAssignment::StartUp()
{
	BaseApp::StartUp();
	
	ShaderHandler::LoadShaderProgram("Phys",
		"Data/Shaders/Physics/Phys.vert",
		"Data/Shaders/Physics/Phys.frag");

	ShaderHandler::LoadShaderProgram("PhysX",
		"Data/Shaders/Physics/PhysX.vert",
		"Data/Shaders/Physics/PhysX.frag");

	//Initialise camera
	InitialiseFlyCamera(15.0f, 20.0f, 0.5f,
		glm::vec3(0, 85, 197), glm::vec3(0));

	m_physicsHandle = new PhysHandle();
	m_physicsHandle->StartUp();

	m_physXHandle = new PhysXHandle();
	m_physXHandle->StartUp();

}

void PhysicsAssignment::ShutDown()
{
	m_physicsHandle->ShutDown();
	m_physXHandle->ShutDown();
}

void PhysicsAssignment::Update(const double& a_dt)
{
	BaseApp::Update(a_dt);

#if PHYSTEST
	m_physicsHandle->Update(1/60.0f);
#else
	m_physXHandle->Update(m_pBaseCamera->GetWorldTransform(), 1/60.0f);
#endif
}

void PhysicsAssignment::Render()
{
	BaseApp::Render();
	RenderSkyBox();

	ApplyLightingSetup(glm::vec3(0.25f),
		glm::vec3(10), 1);

#if PHYSTEST
	m_physicsHandle->Render();
#else
	m_physXHandle->Render();
#endif
}