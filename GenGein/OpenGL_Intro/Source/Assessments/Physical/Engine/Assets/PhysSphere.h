#pragma once
#include <glm\glm.hpp>
#include "Assessments\Physical\Engine\RigidBody.h"

class PhysSphere : public RigidBody
{
public:

	~PhysSphere();
	PhysSphere();
	PhysSphere(const glm::vec3 a_pos, const glm::vec3 a_vel,
		const float a_radius, const float a_mass,
		const glm::vec4 a_colour);

	virtual void Render();

	inline const float& GetRadius() 
		{ return m_radius; };
	inline void SetRadius(const float& a_rad) 
	{ m_radius = a_rad; };

private:
	float m_radius;
	glm::vec4 m_colour;
};