#pragma once
#include <glm\glm.hpp>
#include "Assessments\Physics\PhysComps\Assets\RigidBody.h"

class PhysAABB : public RigidBody
{
public:

	~PhysAABB();
	PhysAABB();
	PhysAABB(const glm::vec3 a_pos, const glm::vec3 a_size,
		const float a_mass, BodyType a_bodyID);

	inline const glm::vec3& GetSize() 
		{ return m_size; };
	inline void SetSize(const glm::vec3 a_size)
	{ m_size = a_size; };

private:
	glm::vec3 m_size;
};