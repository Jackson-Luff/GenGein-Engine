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

	m_moveSpeed = 10.0f;
	m_rotSpeed = 1.0f;
	m_jumpVel = 0.0f;
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
	
	if (m_pHitReport->GetPlayerContactNormal().y > 0.0f)
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
	
	// Set up the movement velocity
	PxVec3 playerVel = PxVec3(0);
	glm::vec4 r = Input::Camera::GetRight();
	glm::vec4 f = Input::Camera::GetForward();
	playerVel += PxVec3(r.x, r.y, r.z) * -m_velocity.x; // Horis move
	playerVel += PxVec3(f.x, f.y, f.z) * m_velocity.y; // Vert move

	if (Input::Keyboard::isKeyDown(KEY_W))
		playerVel *= 4.0f;
	else if (Input::Keyboard::isKeyDown(KEY_S))
		playerVel *= 0.25f;

	float minDistance = 0.001f;
	PxControllerFilters filter;
	
	// Make controls relative to player facing
	PxQuat rotation(0, PxVec3(0,1,0));
	
	// Move the Controller
	m_pPlayerController->move(rotation.rotate(playerVel), minDistance, a_dt, filter);
}