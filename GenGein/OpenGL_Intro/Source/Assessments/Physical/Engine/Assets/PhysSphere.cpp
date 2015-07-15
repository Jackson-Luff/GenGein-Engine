#include <AIE\Gizmos.h>
#include "Assessments\Physical\Engine\RigidBody.h"
#include "Engine\Objects\FBXModel.h"
#include "PhysSphere.h"


PhysSphere::~PhysSphere()
{}

PhysSphere::PhysSphere() : RigidBody()
{}

PhysSphere::PhysSphere(const glm::vec3 a_pos, const glm::vec3 a_vel,
	const float a_radius, const float a_mass,
	const glm::vec4 a_colour) : RigidBody(a_pos, a_vel, 0, a_mass)
{
	m_radius = a_radius;
	m_velocity = a_vel;
	m_colour = a_colour;

	SetID(ShapeType::SPHERE);
}

void PhysSphere::Render()
{
	Gizmos::addSphere(m_position, m_radius, 10, 10, m_colour);
}