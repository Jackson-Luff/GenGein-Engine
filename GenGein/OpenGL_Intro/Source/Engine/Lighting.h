#pragma once
#include <glm\glm.hpp>

typedef const char* c_charp;
typedef const unsigned int c_uint;

enum LIGHT_TYPE
{
	AMBIENT_L,
	POINT_L,
	DIRECTIONAL_L,
	SPOT_L
};

class Lighting
{
public:
	Lighting();
	Lighting( c_uint& a_prog, c_charp a_name, const glm::vec3 a_pos, LIGHT_TYPE a_type);
	~Lighting();

	// Getters / Setters for position
	inline const glm::vec3& GetPosition() const
		{ return m_position; }
	inline const glm::vec3 SetPosition( const glm::vec3 a_newPos )
		{ return m_position = a_newPos; }

	void Render();

private:
	c_charp m_name;
	int m_lightPosUniLoc;
	glm::vec3 m_position;
};

