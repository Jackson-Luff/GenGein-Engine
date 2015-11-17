#pragma once
#include <glm\glm.hpp>
#include "RigidBody.h"

class PhysSphere : public RigidBody
{
public:

	~PhysSphere();
	PhysSphere();
	PhysSphere(const glm::vec3 a_pos, const float a_radius,
		const float a_mass, BodyType a_bodyID);

	inline const float& GetRadius() 
		{ return m_radius; };
	inline void SetRadius(const float& a_rad) 
	{ m_radius = a_rad; };

private:
	float m_radius;
};