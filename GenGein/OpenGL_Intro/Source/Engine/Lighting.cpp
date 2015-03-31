
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

#include "Lighting.h"


Lighting::Lighting()
{}

Lighting::Lighting(c_uint& a_prog, c_charp a_name,const glm::vec3 a_pos, LIGHT_TYPE a_type)
{
	switch (a_type)
	{
	case AMBIENT_L:
		m_position = a_pos;
		break;
	case POINT_L:
		m_position =  a_pos;
		break;
	case DIRECTIONAL_L:
		m_position =  a_pos;
		break;
	case SPOT_L:
		m_position =  a_pos;
		break;
	default:
		m_position = a_pos;
		break;
	}

	m_lightPosUniLoc = glGetUniformLocation(a_prog, a_name);
}

Lighting::~Lighting()
{
}

void Lighting::Render()
{
	glUniform3fv(m_lightPosUniLoc, 1, &m_position[0]);
}