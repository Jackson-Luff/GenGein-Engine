#pragma once
#include <glm\glm.hpp>
#include "Extras\LetsGetPhysical\Components\RigidBody.h"

class PhysSphere : public RigidBody
{
public:

	~PhysSphere();
	PhysSphere();
	PhysSphere(const glm::vec3 a_pos, const glm::vec3 a_vel,
		const float a_mass, const float a_radius,
		const glm::vec4 a_colour);

	inline const float& GetRadius() 
		{ return m_radius; };

private:
	virtual void MakeGizmo();

	float m_radius;
	glm::vec4 m_colour;
};