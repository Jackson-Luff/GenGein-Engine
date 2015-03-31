#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\FBOMesh.h"
#include "Engine\Particles\GPUParticleEmitter.h"
#include "Cameras\FlyCamera.h"
#include "Tutorial08.h"

using glm::vec3;
using glm::vec4;

// Default constructor
Tutorial08::Tutorial08()
{}

// Construct with specified window attributes
Tutorial08::Tutorial08(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial08::~Tutorial08()
{}

// Initialise things
void Tutorial08::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();
	
	initColour = glm::vec4(0.0f, 0.3f, 0.9f, 1);
	endColour = glm::vec4(0.2f, 1.0f, 1.0f, 1);

	m_pEmitter = new GPUParticleEmitter();
	m_pEmitter->Initialise(
		2000000,
		0.1f, 15.0f,
		5.0f, 25.0f,
		0.01f, 0.07f,
		glm::vec4(0.0f, 0.3f, 0.9f, 1),
		glm::vec4(0.2f, 1.0f, 1.0f, 1));

	InitialiseFlyCamera(5.0f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	m_pFBOPlane = new FBOMesh();
	m_pFBOPlane->Initialise(512, 512);
}

// Destroy things 
void Tutorial08::ShutDown()
{
	delete m_pFBOPlane;
	//delete m_pFBXmodel;
}

// Updates inputs and incr time
void Tutorial08::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

// Draws the stuff to screen
void Tutorial08::Render()
{
	GLApplication::Render();
	m_pFBOPlane->SaveOffScreenShot();

	m_pEmitter->Render(
		(float)GetDeltaTime(),
		(float)GetElapsedTime(),
		m_pBaseCamera->GetWorldTransform(),
		m_pBaseCamera->GetProjectionView(),
		initColour, endColour,
		glm::vec3(0));

	m_pFBOPlane->Render(m_pBaseCamera->GetProjectionView(), GetWidth(), GetHeight());

	m_pEmitter->Render(
		(float)GetDeltaTime(),
		(float)GetElapsedTime(),
		m_pBaseCamera->GetWorldTransform(),
		m_pBaseCamera->GetProjectionView(),
		initColour, endColour,
		glm::vec3(0));
}
