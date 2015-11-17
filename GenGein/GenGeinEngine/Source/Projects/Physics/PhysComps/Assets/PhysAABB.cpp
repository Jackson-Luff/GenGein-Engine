#include <glm\ext.hpp>
#include "RigidBody.h"
#include "PhysAABB.h"


PhysAABB::PhysAABB() : RigidBody()
{}

PhysAABB::~PhysAABB()
{}

PhysAABB::PhysAABB(const glm::vec3 a_pos, const glm::vec3 a_size, 
	const float a_mass, BodyType a_bodyID) : RigidBody(a_pos, a_mass)
{
	m_size = a_size;
	m_bodyID = a_bodyID;
	m_shapeID = ShapeType::AABB;
}