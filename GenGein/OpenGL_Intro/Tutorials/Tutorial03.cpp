
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\Lighting.h"
#include "Engine\Objects\ObjMesh.h"
#include "Cameras\FlyCamera.h"

#include "Tutorial03.h"

using glm::vec3;
using glm::mat4;

// Default constructor
Tutorial03::Tutorial03() :
	m_pObj(0),
	m_VAO(0)
{}

// Construct with specified window attributes
Tutorial03::Tutorial03( const int a_width, const int a_height, const char* a_title ) 
	: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial03::~Tutorial03()
{}

// Initialise things
void Tutorial03::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();

	ShaderHandler::LoadShaderProgram("MainProgram",
		"Data/Shaders/ObjShader.vert",
		"Data/Shaders/ObjShader.frag");

	//Initialise camera
	InitialiseFlyCamera(10.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	// Create a mesh based on directory given
	m_pObj = new ObjMesh();
	m_pObj->LoadObject( "Data/Objects/Plane", "planeQuads");

	// Create Light(s)
	m_ambientLight = new Lighting(*m_pMainProgramID, "ambientLight", vec3(0.25f, 0.25f, 0.4f), AMBIENT_L);
	m_pointLight = new Lighting(*m_pMainProgramID, "lightPos", vec3(0.0f, 5.0f, 0.0f), POINT_L);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Light X:", TW_TYPE_FLOAT, (void*)&m_pointLight->GetPosition().x);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Light Y:", TW_TYPE_FLOAT, (void*)&m_pointLight->GetPosition().y);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Light Z:", TW_TYPE_FLOAT, (void*)&m_pointLight->GetPosition().z);
}

// Destroy things 
void Tutorial03::ShutDown()
{}

// Updates inputs and incr time
void Tutorial03::Update( const double a_dt )
{
	m_pBaseCamera->Update(a_dt);

	vec3 newLightPos = vec3((20.0f * sin(GetElapsedTime())), 1,1);

	m_pointLight->SetPosition(m_pointLight->GetPosition());
}

// Draws the stuff to screen
void Tutorial03::Render()
{
	GLApplication::Render();

	// Lighting
	m_ambientLight->Render();
	m_pointLight->Render();

	// Action 
	m_pObj->Render();
}