#include "Engine\Cameras\BaseCamera.h"
#include "Assessments\Physical\PhysicsHandle.h"
#include "Assessments\Physical\PhysXHandle.h"

#include "PhysicsAssignment.h"


PhysicsAssignment::PhysicsAssignment() : BaseApp()
{
}

PhysicsAssignment::PhysicsAssignment(const int a_width, const int a_hieght, const char* a_title)
	: BaseApp(a_width, a_hieght, a_title)
{
}

PhysicsAssignment::~PhysicsAssignment()
{
	delete m_physicsHandle;
	//delete m_physXHandle;
}

void PhysicsAssignment::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(15.0f, 20.0f, 0.5f,
		glm::vec3(0, 0, 125), glm::vec3(0, 0, 120));

	m_physicsHandle = new PhysicsHandle();
	m_physicsHandle->StartUp();
}

void PhysicsAssignment::ShutDown()
{
	m_physicsHandle->ShutDown();
}

void PhysicsAssignment::Update(const double& a_dt)
{
	BaseApp::Update(a_dt);
	m_physicsHandle->Update(a_dt);
}

void PhysicsAssignment::Render()
{
	BaseApp::Render();
	RenderSkyBox();

	glm::mat4 projView = m_pBaseCamera->GetProjectionView();
	m_physicsHandle->Render(projView);
}