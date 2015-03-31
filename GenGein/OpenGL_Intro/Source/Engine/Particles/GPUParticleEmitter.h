#pragma once

#include <glm\glm.hpp>

typedef unsigned int uint;

struct GPUParticleVertex;

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();

	void Initialise(
		const uint a_maxParticles,
		const float a_lifeTimeMin, const float a_lifeTimeMax,
		const float a_velocityMin, const float a_velocityMax,
		const float a_startSize, const float a_endSize,
		const glm::vec4& a_startColour, const glm::vec4& a_endColour);

	void Render(float a_dt, float a_incrTime,
		const glm::mat4& a_cameraTransform,
		const glm::mat4& a_projectionView,
		const glm::vec4& a_startColourOvrRid,
		const glm::vec4& a_endingColourOvrRid,
		const glm::vec3& a_spherePos);

	// Get / Set Positon
	inline glm::vec3& GetPosition() { return m_position; }
	inline glm::vec3 SetPosition(const glm::vec3& a_pos) 
	{ return m_position = a_pos; }

protected:

	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();

	GPUParticleVertex* m_particles;

	uint m_maxParticles;
	
	uint m_activeBuffer;
	uint m_vao[2];
	uint m_vbo[2];

	uint m_drawShader;
	uint m_updateShader;

	glm::vec3 m_position;

	float m_lifeSpanMin;
	float m_lifeSpanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	float m_lastDrawTime;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

	//Pre-defined Uniform Locations
	int ProjViewUniLoc;
	int CamTransUniLoc;
	int MinColourAll;
	int MaxColourAll;
	int StartColourUniLoc;
	int EndColourUniLoc;
};

