#pragma once
#include "PhysActor.h"

class PhysPlane : public PhysActor
{
public:
	~PhysPlane();
	PhysPlane();
	PhysPlane(const glm::vec3 a_normal, const float a_dist);

	virtual void Update(const glm::vec3 a_gravity, const double a_dt);
	//
	inline const float& GetDistance() 
		{ return m_distance; };
	//
	inline const glm::vec3& GetNormal()
		{ return m_normal; }

private:
	float m_distance;
	glm::vec3 m_normal;
};