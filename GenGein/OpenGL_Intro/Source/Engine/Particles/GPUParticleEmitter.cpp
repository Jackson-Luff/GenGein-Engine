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

	// Delete shaders
	glDeleteProgram(m_drawShader);
	glDeleteProgram(m_updateShader);
}

void GPUParticleEmitter::Initialise(
	const uint a_maxParticles,
	const float a_lifeTimeMin, const float a_lifeTimeMax,
	const float a_velocityMin, const float a_velocityMax,
	const float a_startSize, const float a_endSize,
	const glm::vec4& a_startColour, const glm::vec4& a_endColour)
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
	m_drawShader = ShaderHandler::GetShader("ParticleShader");

	TextureHandler::LoadTexture(m_drawShader, "pTexture", "Data/Textures/smoke.png");

	// Bind the shader so that we can set -
	// some uniforms that don't change per-frame
	glUseProgram(m_drawShader);

	// Pre-process Uniform Locations
	ProjViewUniLoc = glGetUniformLocation(m_drawShader, "projectionView");
	CamTransUniLoc = glGetUniformLocation(m_drawShader, "cameraTransform");

	// Bind size information for interpolation that won't change
	int loc = glGetUniformLocation(m_drawShader, "sizeStart");
	glUniform1f(loc, m_startSize);
	loc = glGetUniformLocation(m_drawShader, "sizeEnd");
	glUniform1f(loc, m_endSize);

	// Bind colour information for the interpolation that won't change
	loc = glGetUniformLocation(m_drawShader, "colourStart");
	glUniform4fv(loc, 1, &m_startColour[0]);
	loc = glGetUniformLocation(m_drawShader, "colourEnd");
	glUniform4fv(loc, 1, &m_endColour[0]);

	// Bind colour information for the interpolation that won't change
	StartColourUniLoc = glGetUniformLocation(m_drawShader, "colourStrtOvrRid");
	EndColourUniLoc = glGetUniformLocation(m_drawShader, "colourEndOvrRid");
}

void GPUParticleEmitter::CreateUpdateShader()
{
	// Create a shader
	uint vs = ShaderHandler::CreateShader(
		"Data/Shaders/Particles/ParticleUpdate.vert", GL_VERTEX_SHADER);

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

void GPUParticleEmitter::Render(float a_dt,
	float a_incrTime,
	const glm::mat4& a_cameraTransform,
	const glm::mat4& a_projectionView,
	const glm::vec4& a_startColourOvrRid,
	const glm::vec4& a_endColourOvrRid,
	const glm::vec3& a_spherePos)
{	 
	// Update the particles using transform feedback
	glUseProgram(m_updateShader);

	// Bind time info
	int loc = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(loc, a_incrTime);
	loc = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(loc, a_dt);

	// Bind emitter's position
	loc = glGetUniformLocation(m_updateShader, "emitterPosition");
	glUniform3fv(loc, 1, &m_position[0]);

	loc = glGetUniformLocation(m_updateShader, "SpherePos");
	glUniform3fv(loc, 1, &a_spherePos[0]);
	
	// Disable rasterisation
	glEnable(GL_RASTERIZER_DISCARD);

	// Bind the buffer we will update
	glBindVertexArray(m_vao[m_activeBuffer]);

	// Work out the "other" stuff
	uint otherBuffer = (m_activeBuffer + 1) % 2;

	// Bind the buffer we will update into as points 
	// and begin transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);

	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// Disable transform feedback and enable resterization again
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, 0);

	// Draw the particles using the geo shader to billboard them
	glDisable(GL_DEPTH_TEST);
	glUseProgram(m_drawShader);
	TextureHandler::RenderAllTextures();
	glUniformMatrix4fv(CamTransUniLoc, 1,
		false, &a_cameraTransform[0][0]);
	glUniformMatrix4fv(ProjViewUniLoc, 1,
		false, &a_projectionView[0][0]);
	glUniform4fv(StartColourUniLoc, 1, &a_startColourOvrRid[0]);
	glUniform4fv(EndColourUniLoc, 1, &a_endColourOvrRid[0]);
	// Draw particles in the "other" buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);
	//glEnable(GL_DEPTH_TEST);
	// Swap for the next frame
	m_activeBuffer = otherBuffer;
}