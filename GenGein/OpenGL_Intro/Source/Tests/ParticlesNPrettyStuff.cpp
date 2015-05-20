
#include <gl_core_4_4.h>

#include "Engine\Cameras\BaseCamera.h"
#include "Engine\Particles\GPUParticleEmitter.h"

#include "ParticlesNPrettyStuff.h"

ParticlesNPrettyStuff::ParticlesNPrettyStuff()
{
}

ParticlesNPrettyStuff::ParticlesNPrettyStuff(c_int a_width, c_int a_height, c_pChar a_title)
: BaseApp(a_width, a_height, a_title)
{
}

ParticlesNPrettyStuff::~ParticlesNPrettyStuff()
{
}

void ParticlesNPrettyStuff::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(20), glm::vec3(0));

	m_emitter = new GPUParticleEmitter();
	m_emitter->Initialise(
		1000000,
		0.1f, 28.0f,
		1.0f, 10.0f,
		0.01f, 0.2f,
		glm::vec4(1,1,1,1),
		glm::vec4(0, 0.5f, 0.5f, 1));
}

void ParticlesNPrettyStuff::ShutDown()
{
}

void ParticlesNPrettyStuff::Update(const double a_dt)
{
	BaseApp::Update(a_dt);

	if (glfwGetKey(m_pWindow, GLFW_KEY_C))
	{

	}
}

void ParticlesNPrettyStuff::Render()
{
	BaseApp::Render();
	RenderSkyBox();

	ApplyLightingSetup(glm::vec3(),
		glm::vec3(0, 50, 0),
		10.0f);

	m_emitter->Render(
		(float)GetDeltaTime(),
		(float)GetElapsedTime(),
		m_pBaseCamera->GetWorldTransform(),
		glm::vec3(0, 50, 0));
}

