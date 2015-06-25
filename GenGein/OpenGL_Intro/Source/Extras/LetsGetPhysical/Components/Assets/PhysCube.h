#pragma once
#include <glm\glm.hpp>
#include "Extras\LetsGetPhysical\Components\RigidBody.h"

class PhysCube : public RigidBody
{
public:

	~PhysCube();
	PhysCube();
	PhysCube(const glm::vec3 a_pos, const glm::vec3 a_vel,
		const float a_mass, const float a_sideLength,
		const glm::vec4 a_colour);

	
private:
	virtual void MakeGizmo();

	float m_sideLength;
	glm::vec4 m_colour;
};