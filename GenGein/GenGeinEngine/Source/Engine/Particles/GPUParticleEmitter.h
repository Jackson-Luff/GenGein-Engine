#pragma once

#include <glm\glm.hpp>

struct GPUParticleVertex;

using namespace glm;

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();

	void Initialise(
		const unsigned int& a_maxParticles,
		const float& a_lifeTimeMin, const float& a_lifeTimeMax,
		const float& a_velocityMin, const float& a_velocityMax,
		const float& a_startSize, const float& a_endSize,
		const vec4& a_startColour, const vec4& a_endColour);

	void Render(const float& a_dt);

	// Get / Set Positon
	inline const vec3& GetPosition() const 
		{ return m_position; }
	inline vec3 SetPosition(const vec3& a_pos) 
	{ return m_position = a_pos; }

	void SendVariousUniformData(const bool c_down,
		const bool v_down,
		const bool b_down,
		const bool n_down,
		const bool m_down);

protected:

	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();

	GPUParticleVertex* m_particles;

	unsigned int m_maxParticles;
	
	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];

	unsigned int* m_drawShader;
	unsigned int m_updateShader;

	vec3 m_position;

	float m_lifeSpanMin;
	float m_lifeSpanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	float m_lastDrawTime;

	vec4 m_startColour;
	vec4 m_endColour;

	//Pre-defined Uniform Locations
	int MinColourAll;
	int MaxColourAll;
};

