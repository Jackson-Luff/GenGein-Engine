#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "RigidBody.h"

#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ROTATION_THRESHOLD 0.01f

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
	m_linearDrag = 0.9975f;
}

void RigidBody::Update(const glm::vec3 a_gravity, const double a_dt)
{
	// Applying Gravity
	m_linearVelocity += a_gravity;

	// Linear Velocity and drag
	m_position += m_linearVelocity * a_dt;
	//m_linearVelocity *= m_linearDrag;

	// Angular Velocity and drag
	float rot = 0.1f;
	glm::mat4 rotMatrix = glm::rotate(rot, m_rotation);
	m_angularVelocity *= m_angularDrag;
	m_rotation += m_angularVelocity * a_dt;
	
	if (glm::length(m_linearVelocity) < MIN_LINEAR_THRESHOLD)
		{ m_linearVelocity = glm::vec3(0); }
	
	glm::vec3 absoluteAngVel = glm::abs(m_angularVelocity);
	
	for (int i = 0; i < 3; i++)
	{
		if (absoluteAngVel[i] < MIN_ROTATION_THRESHOLD)
			m_angularVelocity[i] = 0.0f;
	}
}

void RigidBody::ApplyForce(const glm::vec3 a_force)
{
	m_linearVelocity += a_force / m_mass;
}

void RigidBody::ApplyForceToActor(RigidBody* const a_actorB, const glm::vec3 a_force)
{
	m_linearVelocity -= a_force / m_mass;
	a_actorB->m_linearVelocity += a_force / a_actorB->m_mass;
}

void RigidBody::ResetPosition()
{
	m_position = m_origin;
	m_linearVelocity = glm::vec3(0);
}