#include <AIE\Gizmos.h>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Culling.h"
#include "Cameras\FlyCamera.h"

#include "Tutorial07.h"

using glm::vec3;
using glm::vec4;

// Default constructor
Tutorial07::Tutorial07()
{}

// Construct with specified window attributes
Tutorial07::Tutorial07(const int a_width, const int a_height, const char* a_title)
	: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial07::~Tutorial07()
{}

// Initialise things
void Tutorial07::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();
	/*
	m_pShader->CreateGLProgram(m_shaderID,
		"Data/Shaders/FbxShader.vert",
		"Data/Shaders/FbxShader.frag");*/
	
	//Initialise camera
	InitialiseFlyCamera(10.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	Gizmos::create();
}

// Destroy things 
void Tutorial07::ShutDown()
{
}

// Updates inputs and incr time
void Tutorial07::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

// Draws the stuff to screen
void Tutorial07::Render()
{
	GLApplication::Render();
	Gizmos::clear();

	BoundingSphere sphere;
	sphere.SetCenter( vec3(0, cosf((float)GetElapsedTime()) + 1, 0) );
	sphere.SetRadius(0.5f);

	vec4 plane(0, 1, 0, -1);

	float d = glm::dot(vec3(plane), sphere.GetCenter()) + plane.w;

	Gizmos::addSphere(sphere.GetCenter(), sphere.GetRadius(),
									8, 8, vec4(1, 0, 1, 1));
	
	vec4 planeColour(1, 0, 1, 1);

	if (d > sphere.GetRadius())
		planeColour = vec4(0, 0, 1, 1);

	else if (d < -sphere.GetRadius())
		planeColour = vec4(1, 0, 0, 1);

	Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4),
		vec3(-4, 1, 4), planeColour);

	Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4),
		vec3(-4, 1, -4), planeColour);

	vec4 planes[6];
	GetFrustumPlane(m_pBaseCamera->GetProjectionView(), planes);

	for (int i = 0; i < 6; i++) {

		float d = glm::dot(vec3(planes[i]), sphere.GetCenter()) + planes[i].w;

		if (d < -sphere.GetRadius())
			printf("Behind, don't render it!\n");
		else if (d < sphere.GetRadius())
			printf("Touching, so we still wanna render it!\n");
		else
			printf("Front, fully visible so render it!\n");
	}

	// Camera
	Gizmos::draw(m_pBaseCamera->GetProjectionView());
}

// Gets a virtual frustum cube
void Tutorial07::GetFrustumPlane(const glm::mat4& a_transform, glm::vec4* planes)
{
	// Right side
	planes[0] = vec4(a_transform[0][3] - a_transform[1][0],
					 a_transform[1][3] - a_transform[1][0],
					 a_transform[2][3] - a_transform[2][0],
					 a_transform[3][3] - a_transform[3][0]);

	// Left side
	planes[1] = vec4(a_transform[0][3] + a_transform[0][0],
				     a_transform[1][3] + a_transform[1][0],
				     a_transform[2][3] + a_transform[2][0],
				     a_transform[3][3] + a_transform[3][0]);

	// Top
	planes[2] = vec4(a_transform[0][3] - a_transform[0][1],
					 a_transform[1][3] - a_transform[1][1],
					 a_transform[2][3] - a_transform[2][1],
					 a_transform[3][3] - a_transform[3][1]);

	// Bottom
	planes[3] = vec4(a_transform[0][3] + a_transform[0][1],
					 a_transform[1][3] + a_transform[1][1],
					 a_transform[2][3] + a_transform[2][1],
					 a_transform[3][3] + a_transform[3][1]);
	// Far
	planes[0] = vec4(a_transform[0][3] - a_transform[0][2],
					 a_transform[1][3] - a_transform[1][2],
					 a_transform[2][3] - a_transform[2][2],
					 a_transform[3][3] - a_transform[3][2]);
	// Near												
	planes[0] = vec4(a_transform[0][3] - a_transform[0][2],
					 a_transform[1][3] - a_transform[1][2],
					 a_transform[2][3] - a_transform[2][2],
					 a_transform[3][3] - a_transform[3][2]);

	for (int i = 0; i < 6; i++)
		planes[i] = glm::normalize(planes[i]);
}