#include <stack>
#include <gl_core_4_4.h>
#include <glm\ext.hpp>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Cameras\BaseCamera.h"

#include "Tutorial13.h"

Tutorial13::Tutorial13()
{
}

// Construct with specified window attributes
Tutorial13::Tutorial13(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

Tutorial13::~Tutorial13()
{
}

// Initialise Member Variables
void Tutorial13::StartUp()
{
	GLApplication::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(0, 0.5, 0), glm::vec3(-1, 0, -1));

	ShaderHandler::LoadShaderProgram("PBL",
		"Data/Shaders/Geometry/PBLShader.vert",
		"Data/Shaders/Geometry/PBLShader.frag");
	m_pMainProgramID = &ShaderHandler::GetShader("PBL");

	m_model = new FBXModel(vec3(0));
	m_model->LoadFBX(*m_pMainProgramID, 
		"Data/Models/soulspear/soulspear.fbx", FBXFile::UNITS_CENTIMETER);	
}

// Destroy things
void Tutorial13::ShutDown()
{

}

// Update loop
void Tutorial13::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

// Render things to screen
void Tutorial13::Render()
{
	GLApplication::Render();
	glUseProgram(*m_pMainProgramID);
	m_model->Render(mat4(1));
}