#include <AIE\Gizmos.h>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\FBO_RenderScreen.h"
#include "Cameras\FlyCamera.h"
#include "Tutorial09.h"

using glm::vec3;
using glm::vec4;

// Default constructor
Tutorial09::Tutorial09()
{}

// Construct with specified window attributes
Tutorial09::Tutorial09(const int a_width, const int a_height, const char* a_title)
	: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial09::~Tutorial09()
{}

// Initialise things
void Tutorial09::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();

	initColour = glm::vec4(0.0f, 0.3f, 0.9f, 1);
	endColour = glm::vec4(0.2f, 1.0f, 1.0f, 1);

	//Initialise camera
	InitialiseFlyCamera(5.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	m_pRenderScreen = new FBO_RenderScreen();
	m_pRenderScreen->Initialise(GetWidth(), GetHeight());

	Gizmos::create();
}

// Destroy things 
void Tutorial09::ShutDown()
{
}

// Updates inputs and incr time
void Tutorial09::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);

	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));

	Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 8, 8, vec4(1, 1, 0, 1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(
			vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(
			vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
}

// Draws the stuff to screen
void Tutorial09::Render()
{
	GLApplication::Render();

	m_pRenderScreen->SaveOffScreenShot();

	Gizmos::draw(m_pBaseCamera->GetProjectionView());

	m_pRenderScreen->Render();
}
