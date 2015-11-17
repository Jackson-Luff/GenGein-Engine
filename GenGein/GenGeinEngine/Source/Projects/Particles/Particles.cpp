#include "Engine\Renderer\SkyBox.h"
#include "Engine\Particles\GPUParticleEmitter.h"

#include "Particles.h"

Particles::Particles() : BaseApp()
{}

Particles::Particles(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

Particles::~Particles()
{
	delete m_emitter;
}

void Particles::StartUp()
{
	BaseApp::StartUp();

	m_pSkyBox = new SkyBox();
	m_pSkyBox->Create(SkyBox::SPACE);

	//Initialise camera
	InitialiseFlyCamera(15.0f, 20.0f, 0.5f,
		glm::vec3(0, 5, 11), glm::vec3(0));

	m_emitter = new GPUParticleEmitter();
	m_emitter->Initialise(1000000,
		0.7f, 18.0f, 0.1f, 18.8f, 0.01f, 0.1f,
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0.23f, 0.54f, 0.89f, 1.0f));
}

void Particles::ShutDown()
{
}

void Particles::Update(double a_dt)
{
	BaseApp::Update(a_dt);
}

void Particles::Render()
{
	m_pSkyBox->Render();
	m_emitter->Render((float)GetDeltaTime(), (float)GetElapsedTime());
}