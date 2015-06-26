#include <AIE\Gizmos.h>
#include "Extras\LetsGetPhysical\Components\RigidBody.h"
#include "PhysSphere.h"


PhysSphere::PhysSphere() : RigidBody()
{}

PhysSphere::~PhysSphere()
{}

PhysSphere::PhysSphere(const glm::vec3 a_pos, const glm::vec3 a_vel,
	const float a_mass, const float a_radius,
	const glm::vec4 a_colour) : RigidBody(a_pos, a_vel, 0, a_mass)
{
	m_radius = a_radius;
	m_colour = a_colour;

	SetID(ShapeType::SPHERE);
}

// Private Functions:  

void PhysSphere::MakeGizmo()
{
	Gizmos::addSphere(m_position, m_radius, 10, 10, m_colour);
}