#include "Engine\Input\InputHandle.h"
#include "Engine\Objects\FBXModel.h"
#include "Assessments\Physics\PhysComps\PhysScene.h"
#include "Assessments\Physics\PhysComps\Assets\physAABB.h"
#include "Assessments\Physics\PhysComps\Assets\PhysPlane.h"
#include "Assessments\Physics\PhysComps\Assets\PhysSphere.h"
#include "Assessments\Physics\PhysComps\Assets\PhysSpringJoint.h"
#include "PhysHandle.h"

using BodyType = PhysActor::BodyType;

PhysHandle::PhysHandle()
{}

PhysHandle::~PhysHandle()
{}

void PhysHandle::BuildWalling()
{
	PhysAABB* wall;

	// Left wall
	wall = new PhysAABB(glm::vec3(-110, 10.0f, 0.0f), glm::vec3(0.5f, 20.0f, 55), 6.0f, BodyType::STATIC);
	m_scene->AddActor(wall);
	// Back wall
	wall = new PhysAABB(glm::vec3(-55, 10.0f, -55), glm::vec3(55, 20.0f, 0.5f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(wall);
	// Front curb fence
	wall = new PhysAABB(glm::vec3(-55, 2.0f, 55), glm::vec3(55, 3.0f, 0.5f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(wall);
}

void PhysHandle::BuildPhysGeo()
{
	// Sphere on top of big box
	PhysSphere* sphere = new PhysSphere(glm::vec3(-20.75f - 12.5f, 18.0f, 0.5f), 3.0f, 3.0f, BodyType::DYNAMIC);
	m_scene->AddActor(sphere);
	
	PhysAABB* box;
	
	// Big box
	box = new PhysAABB(glm::vec3(-20.75f - 12.0f, 3.0f, 0.0f), glm::vec3(6.0f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(box);
	// Small box infront
	box = new PhysAABB(glm::vec3(-20.75f - 13.0f, 1.325f, 4.45f), glm::vec3(3.0f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(box);
	// Small box to side
	box = new PhysAABB(glm::vec3(-20.75f - 7.5f, 1.325f, 4.0f), glm::vec3(3.0f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(box);

}

void PhysHandle::StartUp()
{
	m_scene = new PhysScene(glm::vec3(0, -9.8f, 0));

	BuildWalling();

	BuildPhysGeo();

	PhysActor* base = new PhysPlane(glm::vec3(0,1,0), 0.0f);
	m_scene->AddActor(base);
}
	
void PhysHandle::ShutDown()
{
	m_scene->RemoveAllActors();
	delete m_scene;
}

void PhysHandle::Update(const double a_dt)
{
	m_scene->Update(a_dt);
	glm::vec3 pos = Input::Camera::GetCamPos();
	Input::Camera::SetCamPos(glm::vec3(pos.x, 7, pos.z));
	if (Input::Keyboard::isKeyDown(KEY_R))
	{
		m_scene->Reset();
	}

	if (Input::Cursor::isLeftButtonDown())
	{
		glm::mat4 camWorld = Input::Camera::GetCameraWorldTrans();
		PhysSphere* ball = new PhysSphere(glm::vec3(camWorld[3]), 1.0f, 1.0f, BodyType::DYNAMIC);
		ball->SetLinearVel(glm::vec3(camWorld[2]) * 10.0f);
		m_scene->AddActor(ball);
	}
}

void PhysHandle::Render()
{
	m_scene->Render();
}