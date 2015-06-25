#include <AIE\Gizmos.h>
#include "Extras\LetsGetPhysical\Components\RigidBody.h"
#include "PhysCube.h"


PhysCube::PhysCube() : RigidBody()
{}

PhysCube::~PhysCube()
{}

PhysCube::PhysCube(const glm::vec3 a_pos, const glm::vec3 a_vel,
	const float a_mass, const float a_sideLength,
	const glm::vec4 a_colour) : RigidBody(a_pos, a_vel, 0, a_mass)
{
	m_sideLength = a_sideLength;
	SetID(ShapeType::CUBE);
}

// Private Functions:

void PhysCube::MakeGizmo()
{
	Gizmos::addAABB(m_position, glm::vec3(m_sideLength), m_colour);
}