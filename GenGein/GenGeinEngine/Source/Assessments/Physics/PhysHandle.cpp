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
{
}

PhysHandle::~PhysHandle()
{}

void PhysHandle::StartUp()
{
	m_scene = new PhysScene(glm::vec3(0, -9.8f, 0));

	PhysSphere* s1 = new PhysSphere(glm::vec3(-10,40,0), 2.0f, 2.0f, BodyType::STATIC);
	s1->SetLinearDrag(0.0f);
	s1->SetElasticity(0.9f);
	m_scene->AddActor(s1);
	
	PhysSphere* s2;
	for (int i = 0; i < 20; i++)
	{
		if (i < 19)
		{
			s2 = new PhysSphere(glm::vec3(i * 6.0f, 40, i * 6.0f), 2.0f, 10.0f, BodyType::DYNAMIC);
			s2->SetLinearDrag(0.1f);
			s2->SetElasticity(1.0f);
		}
		else
		{
			s2 = new PhysSphere(glm::vec3(i * 6.0f, 40, i * 6.0f), 2.0f, 10.0f, BodyType::DYNAMIC);
			s2->SetLinearDrag(0.1f);
			s2->SetElasticity(1.0f);
		}
			
		m_scene->AddActor(s2);
		PhysSpringJoint* sp2 = new PhysSpringJoint(s1, s2, 1.0f, 0.9999f, BodyType::DYNAMIC);
		m_scene->AddActor(sp2);
		s1 = s2;
	}
	//
	//PhysAABB* s1 = new PhysAABB(glm::vec3(0, 40, 0), glm::vec3(2.0f), 2.0f, BodyType::STATIC);
	//s1->SetLinearDrag(0.0f);
	//s1->SetElasticity(0.9f);
	//m_scene->AddActor(s1);
	//
	//PhysSphere* s2;
	//s2 = new PhysSphere(glm::vec3(0, 30, 0), 2.0f, 2.0f, BodyType::DYNAMIC);
	//s2->SetLinearDrag(0.1f);
	//s2->SetElasticity(0.9f);
	//m_scene->AddActor(s2);
	//
	//PhysSpringJoint* sp2 = new PhysSpringJoint(s1, s2, 0.5f, 0.9999f, BodyType::DYNAMIC);
	//m_scene->AddActor(sp2);

	PhysActor* base = new PhysPlane(glm::vec3(0,1,0), -50.0f);
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

	if (Input::Keyboard::isKeyDown(KEY_R))
	{
		m_scene->Reset();
	}

	if (Input::Keyboard::isKeyDown(KEY_T))
	{
		float x = (float)(rand() % 25);
		float y = (float)(rand() % 50);
		float z = (float)(rand() % 25);

		PhysSphere* ball = new PhysSphere(glm::vec3(x,y,z), 1.0f, 1.0f, BodyType::DYNAMIC);
		m_scene->AddActor(ball);
	}
}

void PhysHandle::Render()
{
	m_scene->Render();
}