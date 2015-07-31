#include "Engine\Input\InputHandle.h"
#include "ControllerHitReport.h"

#include "PhysXPlayerController.h"

using KeyBoard = Input::Keyboard;

PhysXPlayerController::PhysXPlayerController()
{
}

PhysXPlayerController::~PhysXPlayerController()
{
}

void PhysXPlayerController::SetUp(PxScene* a_scene, PxPhysics* a_physics, const PxExtendedVec3 a_position)
{
	m_pHitReport = new ControllerHitReport();
	m_pCharacterManager = PxCreateControllerManager(*a_scene);

	m_pPlayerMaterial = a_physics->createMaterial(0.5f, 0.5f, 0.5f);

	// Describe our controller...
	PxCapsuleControllerDesc desc;
	desc.height = 7.0f;
	desc.radius = 1.75;
	desc.position.set(0,0,0);
	desc.material = m_pPlayerMaterial;

	// Connect it to our collision detection routine
	desc.reportCallback = m_pHitReport;
	// Create the player controller
	m_pPlayerController = m_pCharacterManager->createController(desc);

	m_pPlayerController->setPosition(a_position);

	// Initalise the contact normal (what we are in contact with)
	m_pHitReport->ClearPlayerContactNormal();

	m_pPlayerActor = m_pPlayerController->getActor();

	m_movementSpeed = 10.0f;
	m_rotationSpeed = 1.0f;
	m_velocity = PxVec3(0);
	m_position = PxVec3(a_position.x, a_position.y, a_position.z);
}

void PhysXPlayerController::Shutdown()
{

}

void PhysXPlayerController::Update(const PxVec3 a_gravity, const float a_dt)
{
	m_velocity *= 0.5f;
	bool onGround;

	if (Input::Cursor::isRightButtonDown())
	{
		glm::dvec2 prev = Input::Cursor::GetCursorPos();
		glm::dvec2 cur = Input::Cursor::GetCursorPos();
		
		double delta = glm::length(cur - prev);
	
		m_rotation += (float)delta + m_rotationSpeed * a_dt;
	}

	if (m_pHitReport->GetPlayerContactNormal().y > 0.3f)
	{
		m_velocity.y = -0.1f;
		onGround = true;
	}
	else
	{
		m_velocity += a_gravity * a_dt;
		onGround = false;
	}
	
	m_pHitReport->ClearPlayerContactNormal();

	const PxVec3 up(0, 1, 0);

	// Check if we have contact with the normal
	// If y is greater than .3 we assume this is solid ground
	// #IMPROVE THIS SHIT
	//if (m_position.y > 0.3)
	//{
	//	m_velocity += a_gravity * a_dt;
	//	onGround = false;
	//}

	glm::mat4 c = Input::Camera::GetCameraWorldTrans();

	PxVec4 one(c[0].x, c[0].y, c[0].z, c[0].w);
	PxVec4 two(c[1].x, c[1].y, c[1].z, c[1].w);
	PxVec4 three(c[2].x, c[2].y, c[2].z, c[2].w);
	PxVec4 four(c[3].x, c[3].y, c[3].z, c[3].w);

	PxMat44 cam(one, two, three, four);

	PxTransform camTrans(cam);

	// get [0] and [2] and y = 0 on both
	// normalize and scale by speed
	// 

	// Scans the keys and set up our intended velocity
	if (KeyBoard::isKeyDown(KEY_LEFT))
		m_velocity.x -= m_movementSpeed * a_dt;
	else if (KeyBoard::isKeyDown(KEY_RIGHT))
		m_velocity.x += m_movementSpeed * a_dt;
	if (KeyBoard::isKeyDown(KEY_UP))
	{
		PxVec3 dir(0,0,1);
		m_velocity = camTrans.rotate(dir);
		m_velocity = dir;
	}
	else if (KeyBoard::isKeyDown(KEY_DOWN))
		m_velocity.z += m_movementSpeed * a_dt;

	float minDistance = 0.001f;
	PxControllerFilters filter;

	// Make controls relative to player facing
	PxQuat rotation(m_rotation, PxVec3(0, 1, 0));

	// Move the Controller
	m_pPlayerController->move(m_velocity, minDistance, a_dt, filter);
}