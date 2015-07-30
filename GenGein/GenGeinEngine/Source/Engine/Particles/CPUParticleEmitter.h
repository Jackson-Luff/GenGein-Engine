#pragma once

#include <glm\glm.hpp>

typedef unsigned int uint;

struct ParticleData;

class CPUParticleEmitter
{
public:
	
	CPUParticleEmitter();
	virtual ~CPUParticleEmitter();

	void Initialise(
		const uint a_maxParticles, const uint a_emitRate,
		const float a_lifeTimeMin, const float a_lifeTimeMax,
		const float a_velocityMin, const float a_velocityMax,
		const float a_startSize, const float a_endSize,
		const glm::vec4& a_startColour, const glm::vec4& a_endColour);

	void Emit();

	// Get / Set Positon
	inline glm::vec3& GetPosition() { return m_position; }
	inline glm::vec3 SetPosition(const glm::vec3& a_pos)
	{
		return m_position = a_pos;
	}

	// Set all colours on a based colour
	inline void SetColourToAll(const glm::vec4& a_colour)
	{
		for (uint i = 0; i < m_maxParticles; ++i)
		{
			m_vertexData[i].colour += a_colour;
		}
	}

	void Update(float a_dt, const glm::mat4& a_cameraTransform);
	void Draw();
protected:

	struct ParticleVertex {
		glm::vec4 position;
		glm::vec4 colour;
	};
private:

	void InitialiseGLBuffers();

	ParticleData* m_particles;
	uint m_firstDead;
	uint m_maxParticles;

	uint m_vao, m_vbo, m_ibo;

	ParticleVertex* m_vertexData;

	// 

	glm::vec3 m_position;

	float m_emitTimer;
	float m_emitRate;
 
	float m_lifeSpanMin;
	float m_lifeSpanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;
};

