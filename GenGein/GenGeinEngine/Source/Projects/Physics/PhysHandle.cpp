#include "Engine\Input\InputHandle.h"
#include "Engine\Objects\FBXModel.h"
#include "Projects\Physics\PhysComps\PhysScene.h"
#include "Projects\Physics\PhysComps\Assets\physAABB.h"
#include "Projects\Physics\PhysComps\Assets\PhysPlane.h"
#include "Projects\Physics\PhysComps\Assets\PhysSphere.h"
#include "Projects\Physics\PhysComps\Assets\PhysSpringJoint.h"
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
	wall = new PhysAABB(glm::vec3(-110, 10.0f, 0.0f), glm::vec3(1.0f, 20.0f, 55), 6.0f, BodyType::STATIC);
	m_scene->AddActor(wall);
	// Back wall
	wall = new PhysAABB(glm::vec3(-55, 10.0f, -55), glm::vec3(55, 20.0f, 1.0f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(wall);
	// Front curb fence
	wall = new PhysAABB(glm::vec3(-55, 2.0f, 55), glm::vec3(55, 3.0f, 1.0f), 6.0f, BodyType::STATIC);
	m_scene->AddActor(wall);
}

void PhysHandle::BuildPhysGeo()
{
	// Sphere on top of big box
	PhysSphere* sphere = new PhysSphere(glm::vec3(-43.0f, 30.0f, 8.45f), 1.0f, 1.0f, BodyType::DYNAMIC);
	m_scene->AddActor(sphere);
	
	PhysAABB* box;
	
	// Big box
	box = new PhysAABB(glm::vec3(-20.75f - 20.0f, 5.0f, -17.0f), glm::vec3(6.0f), 12.0f, BodyType::DYNAMIC);
	m_scene->AddActor(box);
	box = new PhysAABB(glm::vec3(-20.75f - 20.0f, 20.0f, -17.0f), glm::vec3(3.0f), 6.0f, BodyType::DYNAMIC);
	m_scene->AddActor(box);
	// Small box infront
	box = new PhysAABB(glm::vec3(-43.0f, 4.6f, 8.45f), glm::vec3(3.0f), 6.0f, BodyType::DYNAMIC);
	m_scene->AddActor(box);
	// Small box to side
	box = new PhysAABB(glm::vec3(-20.75f - 7.5f, 4.6f, 4.0f), glm::vec3(3.0f), 6.0f, BodyType::DYNAMIC);
	m_scene->AddActor(box);

	// Joint links
	glm::vec3 pos(-88, 25.0f, -35);
	PhysSphere* b1 = new PhysSphere(pos, 2.0f, 2.0f, BodyType::STATIC);
	b1->SetLinearDrag(0.0f);
	b1->SetElasticity(0.9f);
	m_scene->AddActor(b1);

	PhysSphere* b2;
	for (int i = 0; i < 8; i++)
	{
		if (i < 7)
		{
			b2 = new PhysSphere(pos + glm::vec3(+3 + 6 * i, -5,0), 1.0f, 1.0f, BodyType::DYNAMIC);
			m_scene->AddActor(b2);
		}
		else
		{
			b2 = new PhysSphere(pos + glm::vec3(6 * i, 0,0), 2.0f, 2.0f, BodyType::STATIC);
			m_scene->AddActor(b2);
		}
		
		PhysSpringJoint* spring = new PhysSpringJoint(b1, b2, 25.0f, 0.5f, BodyType::DYNAMIC);
		m_scene->AddActor(spring);
		b1 = b2;
	}		
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
		glm::vec3 pos = Input::Camera::GetCamPos();
		glm::vec3 lookAt = glm::vec3(Input::Camera::GetForward());
		pos += lookAt * 2.0f;
		pos.y += 2.0f;

		float size = abs(sin(Input::Time::GetElapsedTime())) + 0.4f;
		PhysSphere* ball = new PhysSphere(pos, size, size, BodyType::DYNAMIC);
		ball->SetLinearVel(glm::normalize(-lookAt) * 150.0f);
		m_scene->AddActor(ball);
	}
}

void PhysHandle::Render()
{
	m_scene->Render();
}