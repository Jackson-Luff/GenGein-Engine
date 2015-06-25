#include <AIE\Gizmos.h>
#include "Engine\Cameras\BaseCamera.h"
#include "Extras\LetsGetPhysical\Components\PhysScene.h"
#include "Extras\LetsGetPhysical\Components\Assets\PhysCube.h"
#include "Extras\LetsGetPhysical\Components\Assets\PhysSphere.h"
#include "Extras\LetsGetPhysical\Components\Assets\PhysPlane.h"
#include "PhysicsTest01.h"

PhysicsTest01::PhysicsTest01()
{}

PhysicsTest01::PhysicsTest01(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

PhysicsTest01::~PhysicsTest01()
{}

void PhysicsTest01::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(5), glm::vec3(0));

	m_scene = new PhysScene(glm::vec3(0, -10, 0));

	PhysActor* ball = new PhysSphere(
		glm::vec3(0,5,0), glm::vec3(0), 10, 1,
		glm::vec4(0,0.7f,0.8f,1));

	PhysActor* plane = new PhysPlane(glm::vec3(0,1,0), 1);

	m_scene->AddActor(ball);
	m_scene->AddActor(plane);

	Gizmos::create();
}

void PhysicsTest01::ShutDown()
{
	Gizmos::destroy();
	delete m_scene;
}

void PhysicsTest01::Update(const double& a_dt)
{
	BaseApp::Update(a_dt);
	m_scene->Update((float)a_dt);

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R))
	{
		m_scene->Reset();
	}
}

void PhysicsTest01::Render()
{
	BaseApp::Render();

	Gizmos::clear();
	m_scene->AddGizmos();

	Gizmos::draw(m_pBaseCamera->GetProjectionView());
}