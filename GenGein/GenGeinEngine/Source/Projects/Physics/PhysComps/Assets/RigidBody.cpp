#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "RigidBody.h"

#define MIN_LINEAR_THRESHOLD 0.1f
#define MAX_LINEAR_THRESHOLD 100.0f
#define MIN_ROTATION_THRESHOLD 0.01f
#define MAX_ROTATION_THRESHOLD 100.0f

RigidBody::RigidBody() :
	m_mass(0),
	m_elasticity(1),
	m_linearDrag(1),
	m_angularDrag(1),
	m_rotation(glm::vec3(0)),
	m_position(glm::vec3(0)),
	m_linearVelocity(glm::vec3(0)),
	m_angularVelocity(glm::vec3(0))
{}

RigidBody::~RigidBody()
{}

RigidBody::RigidBody(const glm::vec3 a_pos, const float a_mass) 
	: RigidBody()
{
	m_mass = a_mass;
	m_position = a_pos;
	m_origin = a_pos;
	m_linearDrag = 0.98f;
	m_angularDrag = 0.98f;
	m_angularVelocity = glm::vec3(0);
}

void RigidBody::Update(const glm::vec3 a_gravity, const double a_dt)
{
	// Applying Gravity
	m_linearVelocity += a_gravity;

	// Linear Velocity and drag
	m_position += m_linearVelocity * a_dt;
	m_linearVelocity *= m_linearDrag;

	// Angular Velocity and drag
	
	m_rotation += m_angularVelocity * a_dt;
	m_angularVelocity *= m_angularDrag;
	
	for (int i = 0; i < 3; i++)
	{
		if (glm::abs(m_linearVelocity[i]) < MIN_ROTATION_THRESHOLD)
			m_linearVelocity[i] = 0.0f;

		if (glm::abs(m_linearVelocity[i]) > MAX_ROTATION_THRESHOLD)
			m_linearVelocity[i] *= m_angularDrag;

		if (glm::abs(m_angularVelocity[i]) < MIN_ROTATION_THRESHOLD)
			m_angularVelocity[i] = 0.0f;

		if (glm::abs(m_angularVelocity[i]) > MAX_ROTATION_THRESHOLD)
			m_angularVelocity[i] *= 0.5f;
	}
}

void RigidBody::ApplyForce(const glm::vec3 a_force)
{
	if (m_bodyID == BodyType::STATIC)
		return;

	m_linearVelocity += a_force / m_mass;
}

void RigidBody::ApplyForceToActor(RigidBody* const a_actorB, const glm::vec3 a_force)
{
	if (m_bodyID == BodyType::STATIC)
		return;

	m_linearVelocity -= a_force / m_mass;
	a_actorB->m_linearVelocity += a_force / a_actorB->m_mass;
}

void RigidBody::ApplyTorque(float a_torque, glm::vec3 a_dir)
{
	if (m_bodyID == BodyType::STATIC)
		return;

	m_angularVelocity += ((a_torque * a_dir) /m_mass);
}

void RigidBody::ResetPosition()
{
	m_position = m_origin;
	m_linearVelocity = glm::vec3(0);
	m_rotation = glm::vec3(0);
}