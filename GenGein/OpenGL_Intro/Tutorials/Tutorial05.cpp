#include <string>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Particles\CPUParticleEmitter.h"
#include "Engine\GUI\AntTweak.h"

#include "Cameras\FlyCamera.h"

#include "Tutorial05.h"

using glm::vec3;
using glm::mat4;

// Default constructor
Tutorial05::Tutorial05()
{}

// Construct with specified window attributes
Tutorial05::Tutorial05(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial05::~Tutorial05()
{}

// Initialise things
void Tutorial05::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();
	
	ShaderHandler::LoadShaderProgram("MainProgram",
		"Data/Shaders/ParticleShader.vert",
		"Data/Shaders/ParticleShader.frag");

	m_emitter = new CPUParticleEmitter();
	m_emitter->Initialise(
		10000, 200, 0.5f, 1.1f,
		5, 25, 0.1f, 0.01f,
		glm::vec4(0, 0, 0.5f, 1),
		glm::vec4(0, 1, 1, 1));

	//Initialise camera
	InitialiseFlyCamera(10.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	m_overallParticleColour = glm::vec4(0);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Particle Over-all colour", TW_TYPE_COLOR4F, (void*)&m_overallParticleColour);
}

// Destroy things 
void Tutorial05::ShutDown()
{
}

// Updates inputs and incr time
void Tutorial05::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
	m_emitter->Update((float)a_dt, m_pBaseCamera->GetWorldTransform());

	m_emitter->SetColourToAll(m_overallParticleColour);
}

// Draws the stuff to screen
void Tutorial05::Render()
{
	GLApplication::Render();
	
	// Particle Emitter
	m_emitter->Draw();
}