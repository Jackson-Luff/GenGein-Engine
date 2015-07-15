#include <AIE\Gizmos.h>
#include "Engine\Objects\FBXModel.h"
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

void PhysPlane::Render()
{
	float lineSegLngth = 75;
	glm::vec3 centerPoint = m_normal * m_distance;
	glm::vec3 parallel = glm::vec3(m_normal.y, -m_normal.x, 0);

	glm::vec3 start = centerPoint + (parallel * lineSegLngth);
	glm::vec3 end = centerPoint - (parallel * lineSegLngth);

	Gizmos::addLine(start, end, glm::vec4(1));
}