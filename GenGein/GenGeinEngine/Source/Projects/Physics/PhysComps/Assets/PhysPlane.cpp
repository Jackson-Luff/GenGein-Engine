#include "PhysPlane.h"

PhysPlane::~PhysPlane()
{}

PhysPlane::PhysPlane() :
	m_distance(10),
	m_normal(glm::vec3(0,1,0))
{}

PhysPlane::PhysPlane(const glm::vec3 a_normal, const float a_dist)
{
	m_distance = a_dist;
	m_normal = a_normal;
	m_shapeID = ShapeType::PLANE;
	m_bodyID = BodyType::STATIC;
}

void PhysPlane::Update(const glm::vec3 a_gravity, const double a_dt)
{}