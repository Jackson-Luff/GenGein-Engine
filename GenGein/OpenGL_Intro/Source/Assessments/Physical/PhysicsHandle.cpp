#include "Engine\Input\InputHandle.h"

#include "Assessments\Physical\Engine\PhysScene.h"
#include "Assessments\Physical\Engine\Assets\PhysAABB.h"
#include "Assessments\Physical\Engine\Assets\PhysPlane.h"
#include "Assessments\Physical\Engine\Assets\PhysSphere.h"
#include "PhysicsHandle.h"


PhysicsHandle::PhysicsHandle()
{
}


PhysicsHandle::~PhysicsHandle()
{
}

void PhysicsHandle::StartUp()
{
	m_scene = new PhysScene(glm::vec3(0, -500, 0));

	//PhysActor* ball0 = new PhysSphere(glm::vec3(-1, 40, 0), glm::vec3(0), 2, 2, glm::vec4(1));
	//PhysActor* ball1 = new PhysSphere(glm::vec3(+0, 36, 0), glm::vec3(0), 3, 3, glm::vec4(1));
	//PhysActor* ball2 = new PhysSphere(glm::vec3(+1, 26, 0), glm::vec3(0), 4, 4, glm::vec4(1));
	//PhysActor* ball3 = new PhysSphere(glm::vec3(+0, 15, 0), glm::vec3(0), 5, 5, glm::vec4(1));

	PhysActor* planeBot = new PhysPlane(glm::vec3(0.1, 1, 0.5), 50.0f);
	PhysActor* planeLft = new PhysPlane(glm::vec3(1, 0, 0), 50.0f);
	PhysActor* planeRgt = new PhysPlane(glm::vec3(1, 0, 0), -50.0f);
	PhysActor* planeTop = new PhysPlane(glm::vec3(0.1, 1, -0.5), -50.0f);

	//m_scene->AddActor(ball0);
	//m_scene->AddActor(ball1);
	//m_scene->AddActor(ball2);
	//m_scene->AddActor(ball3);

	m_scene->AddActor(planeBot);
	m_scene->AddActor(planeLft);
	m_scene->AddActor(planeRgt);
	m_scene->AddActor(planeTop);
}

void PhysicsHandle::ShutDown()
{
	m_scene->RemoveAllActors();
	delete m_scene;
}

void PhysicsHandle::Update(const double a_dt)
{
	m_scene->Update(a_dt);

	if (Input::Keyboard::isKeyDown(KEY_T))
	{
		m_scene->Reset();
	}
}

void PhysicsHandle::Render(const glm::mat4& a_projView)
{
	m_scene->AddGizmos(a_projView);
}