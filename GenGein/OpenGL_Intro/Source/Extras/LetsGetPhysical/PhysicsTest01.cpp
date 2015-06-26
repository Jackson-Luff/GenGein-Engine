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
		glm::vec3(0, 0, 125), glm::vec3(0, 0, 120));

	m_scene = new PhysScene(glm::vec3(0, -500, 0));

	PhysActor* ball0 = new PhysSphere(glm::vec3(-1,40,0), glm::vec3(0), 1, 1, glm::vec4(1));
	PhysActor* ball1 = new PhysSphere(glm::vec3(0,36,0), glm::vec3(0), 3, 3, glm::vec4(1));
	PhysActor* ball2 = new PhysSphere(glm::vec3(1,26,0), glm::vec3(0), 4, 4, glm::vec4(1));
	PhysActor* ball3 = new PhysSphere(glm::vec3(0,15,0), glm::vec3(0), 5, 5, glm::vec4(1));

	PhysActor* planeBot = new PhysPlane(glm::vec3(0, 1, 0), 50.0f);
	PhysActor* planeLft = new PhysPlane(glm::vec3(1, 0, 0), 50.0f);
	PhysActor* planeRgt = new PhysPlane(glm::vec3(1, 0, 0), -50.0f);
	PhysActor* planeTop = new PhysPlane(glm::vec3(0, 1, 0), -50.0f);

	m_scene->AddActor(ball0);
	m_scene->AddActor(ball1);
	m_scene->AddActor(ball2);
	m_scene->AddActor(ball3);

	m_scene->AddActor(planeBot);
	m_scene->AddActor(planeLft);
	m_scene->AddActor(planeRgt);
	m_scene->AddActor(planeTop);

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
	RenderSkyBox();

	Gizmos::clear();
	m_scene->AddGizmos();
	Gizmos::draw(m_pBaseCamera->GetProjectionView());
}