#include <AIE\Gizmos.h>
#include "Assessments\Physical\Engine\RigidBody.h"
#include "PhysAABB.h"


PhysAABB::PhysAABB() : RigidBody()
{}

PhysAABB::~PhysAABB()
{}

PhysAABB::PhysAABB(const glm::vec3 a_pos, const glm::vec3 a_vel,
	const glm::vec3 a_size, const float a_mass,
	const glm::vec4 a_colour) : RigidBody(a_pos, a_vel, 0, a_mass)
{
	m_size = a_size;
	m_colour = a_colour;
	SetID(ShapeType::AABB);
}

void PhysAABB::Render()
{
	Gizmos::addAABBFilled(m_position, m_size, m_colour);
}