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
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = m_pPlayerMaterial;

	// Connect it to our collision detection routine
	desc.reportCallback = m_pHitReport;
	// Create the player controller
	m_pPlayerController = m_pCharacterManager->createController(desc);

	m_pPlayerController->setPosition(a_position);

	// Initalise the contact normal (what we are in contact with)
	m_pHitReport->ClearPlayerContactNormal();

	m_pPlayerActor = m_pPlayerController->getActor();

	m_velocity = PxVec3(0);
	m_position = PxVec3(0);
}

void PhysXPlayerController::Shutdown()
{

}

void PhysXPlayerController::Update(const PxVec3 a_gravity, const float a_dt)
{
	m_position += m_velocity * a_dt;
	m_velocity *= 0.5f;

	const PxVec3 up(0, 1, 0);

	bool onGround;
	float movementSpeed = 30.0f;
	float rotationSpeed = 1.0f;
	float playerRotation = 0.0f;

	// Check if we have contact with the normal
	// If y is greater than .3 we assume this is solid ground
	// #IMPROVE THIS SHIT

	{
		m_velocity += a_gravity * a_dt;
		onGround = false;
	}

	// Scans the keys and set up our intended velocity 
	if (KeyBoard::isKeyDown(KEY_LEFT))
		m_velocity.x -= movementSpeed * a_dt;
	else if (KeyBoard::isKeyDown(KEY_RIGHT))
		m_velocity.x += movementSpeed * a_dt;
	if (KeyBoard::isKeyDown(KEY_UP))
		m_velocity.z -= movementSpeed * a_dt;
	else if (KeyBoard::isKeyDown(KEY_DOWN))
		m_velocity.z += movementSpeed * a_dt;

	float minDistance = 0.001f;
	PxControllerFilters filter;

	// Make controls relative to player facing
	PxQuat rotation(playerRotation, PxVec3(0, 1, 0));

	// Move the Controller
	m_pPlayerController->move(rotation.rotate(m_position), minDistance, a_dt, filter);
}