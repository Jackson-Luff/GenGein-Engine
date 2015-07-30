#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\TextureHandler.h"
#include "ParticleTypes.h"
#include "GPUParticleEmitter.h"

// REMOVE
#include <fstream>

GPUParticleEmitter::GPUParticleEmitter() :
m_particles(nullptr), m_maxParticles(0),
m_position(0, 0, 0),
m_drawShader(0), m_updateShader(0),
m_lastDrawTime(0) {
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;
}

GPUParticleEmitter::~GPUParticleEmitter()
{
	delete[] m_particles;

	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);

	glDeleteProgram(m_updateShader);
}

void GPUParticleEmitter::Initialise(
	const uint32_t& a_maxParticles,
	const float32_t& a_lifeTimeMin, const float32_t& a_lifeTimeMax,
	const float32_t& a_velocityMin, const float32_t& a_velocityMax,
	const float32_t& a_startSize, const float32_t& a_endSize,
	const f32vec4& a_startColour, const f32vec4& a_endColour)
{

	// Store all variables passed in 
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifeSpanMin = a_lifeTimeMin;
	m_lifeSpanMax = a_lifeTimeMax;
	m_maxParticles = a_maxParticles;

	// Create particle array
	m_particles = new GPUParticleVertex[a_maxParticles];

	// set our starting ping-pong buffer
	m_activeBuffer = 0;

	// Creation of shaders / buff info
	CreateBuffers();
	CreateUpdateShader();
	CreateDrawShader();
}

void GPUParticleEmitter::CreateBuffers()
{
	// Create OpenGL buffers
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(GPUParticleVertex), m_particles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // Positions
	glEnableVertexAttribArray(1); // Colours
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0) + 28);

	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(GPUParticleVertex), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // Positions
	glEnableVertexAttribArray(1); // Colours
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(GPUParticleVertex), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUParticleEmitter::CreateDrawShader()
{
	ShaderHandler::LoadShaderProgram("ParticleShader",
		"Data/Shaders/Particles/Particle.vert",
		"Data/Shaders/Particles/Particle.frag",
		"Data/Shaders/Particles/Particle.geom");
	m_drawShader = &ShaderHandler::GetShader("ParticleShader");

	//TextureHandler::LoadTexture(m_drawShader, "pTexture", "Data/Textures/smoke.png");

	// Bind the shader so that we can set -
	// some uniforms that don't change per-frame
	glUseProgram(*m_drawShader);
	
	// Bind size information for interpolation that won't change
	int loc = glGetUniformLocation(*m_drawShader, "sizeStart");
	glUniform1f(loc, m_startSize);
	loc = glGetUniformLocation(*m_drawShader, "sizeEnd");
	glUniform1f(loc, m_endSize);

	// Bind colour information for the interpolation that won't change
	loc = glGetUniformLocation(*m_drawShader, "colourStart");
	glUniform4fv(loc, 1, &m_startColour[0]);
	loc = glGetUniformLocation(*m_drawShader, "colourEnd");
	glUniform4fv(loc, 1, &m_endColour[0]);
}

void GPUParticleEmitter::CreateUpdateShader()
{
	// Create a shader
	uint vs;
	ShaderHandler::CreateShader(
		"Data/Shaders/Particles/ParticleUpdate.vert", 
		ShaderHandler::ShaderType::VERT_SHADER, vs);

	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, vs);

	// Specify the data that we will stream back
	const char* varyings[] = 
	{ "vPosition", "vVelocity",
	  "vLifetime", "vLifespan" };

	glTransformFeedbackVaryings(m_updateShader,
		4, varyings, GL_INTERLEAVED_ATTRIBS);

	// Bind the shader so that we can set some -
	// uniforms that don't change per-frame
	glLinkProgram(m_updateShader);

	// Remove unneeded handles
	glDeleteShader(vs);

	// Bind the shader so that we can set some 
	// uniforms that don't change per-fram
	glUseProgram(m_updateShader);

	// Bind lifetime minimum and maximum
	int loc = glGetUniformLocation(m_updateShader, "lifeMin");
	glUniform1f(loc, m_lifeSpanMin);
	loc = glGetUniformLocation(m_updateShader, "lifeMax");
	glUniform1f(loc, m_lifeSpanMax);
	loc = glGetUniformLocation(m_updateShader, "veloMin");
	glUniform1f(loc, m_velocityMin);
	loc = glGetUniformLocation(m_updateShader, "veloMax");
	glUniform1f(loc, m_velocityMax);
}

void GPUParticleEmitter::Render(
	const float32_t& a_dt)
{	 
	// Update the particles using transform feedback
	glUseProgram(m_updateShader);

	// Bind time info
	int loc = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(loc, a_dt);

	loc = glGetUniformLocation(m_updateShader, "emitterPosition");
	glUniform3fv(loc, 1, &m_position[0]);

	// Disable rasterisation
	glEnable(GL_RASTERIZER_DISCARD);

	// Bind the buffer we will update
	glBindVertexArray(m_vao[m_activeBuffer]);

	// Work out the "other" stuff
	uint32_t otherBuffer = (m_activeBuffer + 1) % 2;

	// Bind the buffer we will update into as points 
	// and begin transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// Disable transform feedback and enable resterization again
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, 0);

	glUseProgram(*m_drawShader);
	// Draw particles in the "other" buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// Swap for the next frame
	m_activeBuffer = otherBuffer;
}

void GPUParticleEmitter::SendVariousUniformData(const bool c_down,
	const bool v_down,
	const bool b_down, 
	const bool n_down,
	const bool m_down)
{
	int uniloc = glGetUniformLocation(m_updateShader, "C_down");

}