#include "RigidBody.h"

RigidBody::RigidBody() :
	m_rotation2D(0),
	m_mass(0),
	m_position(glm::vec3(0)),
	m_velocity(glm::vec3(0))
{}

RigidBody::~RigidBody()
{}

RigidBody::RigidBody(const glm::vec3 a_pos, const glm::vec3 a_vel, const float a_rot, const float a_mass)
{
	m_rotation2D = a_rot;
	m_mass = a_mass;
	m_position = a_pos;
	m_origin = a_pos;
	m_velocity = a_vel;
}

void RigidBody::Update(const glm::vec3 a_gravity, const float a_dt)
{
	m_position += m_velocity * a_dt;
	//m_velocity *= 0.99f;
	ApplyForce(a_gravity);
}

void RigidBody::Debug()
{}

void RigidBody::ApplyForce(const glm::vec3 a_force)
{
	m_velocity += a_force/m_mass;
}

void RigidBody::ApplyForceToActor(RigidBody* const a_actorB, const glm::vec3 a_force)
{
	m_velocity -= a_force / m_mass;
	a_actorB->m_velocity += a_force / a_actorB->m_mass;
}

void RigidBody::ResetPosition()
{
	m_position = m_origin;
	m_velocity = glm::vec3(0);
}