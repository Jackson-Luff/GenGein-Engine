
#include <vector>
#include <AIE\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Cameras\FlyCamera.h"

#include "Tutorial01.h"

using glm::vec3;
using glm::mat4;

// Default Constructor
Tutorial01::Tutorial01()
{}

// Construct with specified window attributes
Tutorial01::Tutorial01( const int a_width, const int a_height, const char* a_title) 
	: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial01::~Tutorial01()
{}

// Initialise member var's
void Tutorial01::StartUp()
{
	GLApplication::StartUp();

	// Initialise Camera
	InitialiseFlyCamera(10.f, 30.f, 1.0f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	Gizmos::create();
}

// Destroy member var's
void Tutorial01::ShutDown()
{
}

// Update camera input
void Tutorial01::Update( double a_dt )
{	
	m_pBaseCamera->Update(a_dt);
}

// Draw a grid appropriated with an inputted size
void Tutorial01::DrawGrid( unsigned int a_count )
{
	Gizmos::addTransform(glm::mat4(1));
	glm::vec4 white(1);
	glm::vec4 black(0,0,0,1);
	for(unsigned int i = 0; i < a_count; ++i) 
	{
		Gizmos::addLine(
			vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(
			10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
	Gizmos::draw( m_pBaseCamera->GetProjectionView() );
}

// Draw Sphere and grid
void Tutorial01::Render()
{
	Gizmos::clear();

	DrawGrid(21);
	Gizmos::addSphere(vec3(0,0,0), 5, 10, 10, glm::vec4(0,0.5f,0.8f,1));
	
	// Draw to this projection view
	Gizmos::draw( m_pBaseCamera->GetProjectionView() );
}