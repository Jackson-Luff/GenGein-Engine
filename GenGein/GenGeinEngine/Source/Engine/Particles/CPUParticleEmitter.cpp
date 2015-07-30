#include <gl_core_4_4.h>
#include "ParticleTypes.h"
#include "CPUParticleEmitter.h"

CPUParticleEmitter::CPUParticleEmitter()
	: m_particles(nullptr),
	m_firstDead(0),
	m_maxParticles(0),
	m_position(0, 0, 0),
	m_vao(0), m_vbo(0), m_ibo(0),
	m_vertexData(nullptr)
{}

CPUParticleEmitter::~CPUParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void CPUParticleEmitter::Initialise(
	const uint a_maxParticles, const uint a_emitRate,
	const float a_lifeTimeMin, const float a_lifeTimeMax,
	const float a_velocityMin, const float a_velocityMax,
	const float a_startSize, const float a_endSize,
	const glm::vec4& a_startColour, const glm::vec4& a_endColour)
{
	// Set up emit timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / a_emitRate;

	// Store all variables passed in 
	m_startColour =	a_startColour;
	m_endColour =	a_endColour;
	m_startSize =	a_startSize;
	m_endSize =		a_endSize;
	m_velocityMin =	a_velocityMin;
	m_velocityMax =	a_velocityMax;
	m_lifeSpanMin =	a_lifeTimeMin;
	m_lifeSpanMax =	a_lifeTimeMax;
	m_maxParticles = a_maxParticles;

	// Create particle array
	m_particles = new ParticleData[m_maxParticles];
	m_firstDead = 0;

	// Create the array of verticies of particles
	// 4 vertices per particle for a quad.
	// will be filled during update
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	InitialiseGLBuffers();
}

void CPUParticleEmitter::InitialiseGLBuffers()
{
	// Create the indices buffer data of particles
	// 6 indices per quad of 2 tris.
	// fill now as it never changes
	uint* indexData = new uint[m_maxParticles * 6];
	for (uint i = 0; i < m_maxParticles; ++i)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// Create OpenGL buffers
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ARRAY_BUFFER,
		m_maxParticles * sizeof(ParticleVertex) * 4,
		m_vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		m_maxParticles * sizeof(uint) * 6,
		indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Positions
	glEnableVertexAttribArray(1); // Colours

	// Position && Colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
		sizeof(ParticleVertex), ((char*)0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
		sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void CPUParticleEmitter::Emit()
{
	// Only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	// Resurrect the first dead particle
	ParticleData& particle = m_particles[m_firstDead++];

	// Assign it's starting point
	particle.position = m_position;

	// Randomise it's lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / float(RAND_MAX) *
		(m_lifeSpanMax - m_lifeSpanMin) + m_lifeSpanMin);

	// Set starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// Randomise velocity direction and strength
	float velocity = (rand() / float(RAND_MAX) *
		(m_velocityMax - m_velocityMin) + m_velocityMin);

	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

void CPUParticleEmitter::Update(float a_dt, const glm::mat4& a_cameraTransform)
{
	using glm::vec3;
	using glm::vec4;

	// Spawn Particles
	m_emitTimer += a_dt;
	while (m_emitTimer > m_emitRate) {
		Emit();
		m_emitTimer -= m_emitRate;
	}

	uint quad = 0;

	// Update particle and turn live particle into billboard quads
	for (uint i = 0; i < m_firstDead; i++) {
		ParticleData* particle = &m_particles[i];
		particle->lifetime += a_dt;

		if (particle->lifetime >= particle->lifespan) {
			// Swap last alive with this one
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else {

			// Move particle
			particle->position += particle->velocity * a_dt;

			// Size particle 
			particle->size = glm::mix(m_startSize, m_endSize,
				particle->lifetime / particle->lifespan);

			// Colour particle
			particle->colour = glm::mix(m_startColour, m_endColour,
				particle->lifetime / particle->lifespan);

			// Make a quad the correct size and colour
			float halfSize = particle->size * 0.5f;

			m_vertexData[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;

			m_vertexData[quad * 4 + 1].position = vec4(-halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;

			m_vertexData[quad * 4 + 2].position = vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;

			m_vertexData[quad * 4 + 3].position = vec4(halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;

			vec3 zAxis = glm::normalize(vec3(a_cameraTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);

			glm::mat4 billboard(vec4(xAxis, 0), 
								vec4(yAxis, 0), 
								vec4(zAxis, 0),
								vec4(0, 0, 0, 1));

			m_vertexData[quad * 4 + 0].position = billboard * 
				m_vertexData[quad * 4 + 0].position + vec4(particle->position, 0);

			m_vertexData[quad * 4 + 1].position = billboard *
				m_vertexData[quad * 4 + 1].position + vec4(particle->position, 0);

			m_vertexData[quad * 4 + 2].position = billboard *
				m_vertexData[quad * 4 + 2].position + vec4(particle->position, 0);

			m_vertexData[quad * 4 + 3].position = billboard *
				m_vertexData[quad * 4 + 3].position + vec4(particle->position, 0);
			++quad;
		}
	}
}

void CPUParticleEmitter::Draw()
{
	// Sync the particle vertex buffer
	// Based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	// Draw Particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}
