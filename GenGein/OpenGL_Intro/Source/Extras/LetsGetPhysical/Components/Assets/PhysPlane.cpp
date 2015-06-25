#include <AIE\Gizmos.h>

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
	SetID(ShapeType::PLANE);
}

void PhysPlane::Update(const glm::vec3 a_gravity, const float a_dt)
{

}

void PhysPlane::Debug()
{

}

// Private Functions:

void PhysPlane::MakeGizmo()
{
	float lineSegLngth = 300;
	glm::vec2 centerPoint = glm::vec2(m_normal) * m_distance;
	glm::vec2 parallel = glm::vec2(m_normal.y, -m_normal.x);

	glm::vec2 start = centerPoint + (parallel * lineSegLngth);
	glm::vec2 end   = centerPoint - (parallel * lineSegLngth);
	Gizmos::add2DLine(start, end, glm::vec4(1));
}