#include <string>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <FBXFile.h>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Lighting.h"
#include "Engine\Objects\FBXModel.h"
#include "Cameras\FlyCamera.h"

#include "Tutorial04.h"

using glm::vec3;
using glm::mat4;

// Default constructor
Tutorial04::Tutorial04() :
m_pFbxModel(0)
{}

// Construct with specified window attributes
Tutorial04::Tutorial04(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial04::~Tutorial04()
{}

// Initialise things
void Tutorial04::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();
	
	ShaderHandler::LoadShaderProgram("MainShader",
		"Data/Shaders/Geometry/FbxShader.vert",
		"Data/Shaders/Geometry/FbxShader.frag");
	m_pMainProgramID = &ShaderHandler::GetShader("MainShader");

	//Initialise camera
	InitialiseFlyCamera(10.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	// Create a mesh based on directory given
	m_pFbxModel = new FBXModel(vec3(0));
	m_pFbxModel->LoadFBX(*m_pMainProgramID,
		"Data/Models/EnemyElite/EnemyElite.fbx", 
		FBXFile::UNITS_KILOMETER );

	// Create Light(s)
	m_ambientLight = new Lighting(*m_pMainProgramID, "ambientLight", vec3(0.25f, 0.25f, 0.4f), AMBIENT_L);
	m_pointLight = new Lighting(*m_pMainProgramID, "lightPos", vec3(0.0f, 5.0f, 0.0f), POINT_L);
}

// Destroy things 
void Tutorial04::ShutDown()
{
	m_pFbxModel->CleanupOpenGLBuffers();
}

// Updates inputs and incr time
void Tutorial04::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
	m_pFbxModel->Update(GetElapsedTime());
}

// Draws the stuff to screen
void Tutorial04::Render()
{
	GLApplication::Render();
	
	// Lighting
	m_ambientLight->Render();
	m_pointLight->Render();
	// Action ;)
	m_pFbxModel->Render();
}