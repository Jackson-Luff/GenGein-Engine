#include <gl_core_4_4.h>

#include "PhysActor.h"
#include "RigidBody.h"
#include "PhysSpringJoint.h"

PhysSpringJoint::PhysSpringJoint(RigidBody* a_connA, RigidBody* a_connB,
	float a_springCoefficient, float a_damping, 
	BodyType a_bodyID)
{
	m_conns[0] = a_connA;
	m_conns[1] = a_connB;
	m_springCoefficient = a_springCoefficient;
	m_damping = a_damping;

	m_restLength = glm::length(a_connB->GetPosition() - a_connA->GetPosition());
	m_bodyID = a_bodyID;
	m_shapeID = ShapeType::JOINT;
}

PhysSpringJoint::~PhysSpringJoint()
{}

void PhysSpringJoint::Update(const glm::vec3 a_gravity, const double a_dt)
{
	// F - the force the spring exerts
	// K - the spring constant (it's stiffness)
	// X - the displacement of the spring's end from it's resting point
	// b - damping value of spring.
	// v - relative velocity of the particle.
	// Hooke's Law: F = (-K * X) - (b * v)

	float currLength = glm::length(m_conns[0]->GetPosition() - m_conns[1]->GetPosition());

	// Spring Constant
	float K = m_springCoefficient;
	// Displacement
	float X = currLength - m_restLength;
	// Damping
	float b = m_damping;
	// Relative Velocity
	float v = glm::length(m_conns[0]->GetLinearVel() - m_conns[1]->GetLinearVel());

	float force = (-K * X) - (b * v);

	glm::vec3 forceDir = glm::normalize(m_conns[0]->GetPosition() - m_conns[1]->GetPosition());

	if (m_conns[0]->GetBodyID() == BodyType::DYNAMIC)
		m_conns[0]->ApplyForce(force * forceDir);

	if (m_conns[1]->GetBodyID() == BodyType::DYNAMIC)
		m_conns[1]->ApplyForce(-force * forceDir);

	// Should I be applying displacement in vec3's instead? .. I did that and it didn't work
	// Still not working
}