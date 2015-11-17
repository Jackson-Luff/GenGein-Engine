#include "RigidBody.h"
#include "PhysSphere.h"

PhysSphere::~PhysSphere()
{}

PhysSphere::PhysSphere() : RigidBody()
{}

PhysSphere::PhysSphere(const glm::vec3 a_pos, const float a_radius, 
	const float a_mass, BodyType a_bodyID)
	: RigidBody(a_pos, a_mass)
{
	m_radius = a_radius;
	m_bodyID = a_bodyID;
	m_shapeID = ShapeType::SPHERE;
}