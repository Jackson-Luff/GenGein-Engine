#pragma once

#include <glm\glm.hpp>

struct GPUParticleVertex;

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();

	void Initialise(
		const uint32_t& a_maxParticles,
		const float32_t& a_lifeTimeMin, const float32_t& a_lifeTimeMax,
		const float32_t& a_velocityMin, const float32_t& a_velocityMax,
		const float32_t& a_startSize, const float32_t& a_endSize,
		const f32vec4& a_startColour, const f32vec4& a_endColour);

	void Render(const float32_t& a_dt,
		const float32_t& a_incrTime,
		const f32mat4& a_camWorldTrans,
		const f32vec3& a_SunPos);

	// Get / Set Positon
	inline const f32vec3& GetPosition() const 
		{ return m_position; }
	inline f32vec3 SetPosition(const f32vec3& a_pos) 
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

	uint32_t m_maxParticles;
	
	uint32_t m_activeBuffer;
	uint32_t m_vao[2];
	uint32_t m_vbo[2];

	uint32_t* m_drawShader;
	uint32_t m_updateShader;

	f32vec3 m_position;

	float32_t m_lifeSpanMin;
	float32_t m_lifeSpanMax;

	float32_t m_velocityMin;
	float32_t m_velocityMax;

	float32_t m_startSize;
	float32_t m_endSize;

	float32_t m_lastDrawTime;

	f32vec4 m_startColour;
	f32vec4 m_endColour;

	//Pre-defined Uniform Locations
	int32_t MinColourAll;
	int32_t MaxColourAll;
};

