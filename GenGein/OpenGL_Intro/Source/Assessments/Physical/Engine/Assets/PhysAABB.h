#pragma once
#include <glm\glm.hpp>
#include "Assessments\Physical\Engine\RigidBody.h"

class PhysAABB : public RigidBody
{
public:

	~PhysAABB();
	PhysAABB();
	PhysAABB(const glm::vec3 a_pos, const glm::vec3 a_vel,
		const glm::vec3 a_size,	const float a_mass,
		const glm::vec4 a_colour);

	virtual void Render();

	inline const glm::vec3& GetSize() 
		{ return m_size; };
	inline void GetSize(const glm::vec3 a_size)
	{ m_size = a_size; };

private:

	glm::vec3 m_size;
	glm::vec4 m_colour;
};