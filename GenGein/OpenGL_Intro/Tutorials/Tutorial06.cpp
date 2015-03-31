#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\Particles\GPUParticleEmitter.h"
#include "Engine\GUI\AntTweak.h"

#include "Cameras\FlyCamera.h"

#include "Tutorial06.h"

using glm::vec3;
using glm::mat4;

// Default constructor
Tutorial06::Tutorial06()
{}

// Construct with specified window attributes
Tutorial06::Tutorial06(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial06::~Tutorial06()
{}

// Initialise things
void Tutorial06::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();
	
	ShaderHandler::LoadShaderProgram("MainProgram",
		"Data/Shaders/MeshShaders/FbxShader.vert",
		"Data/Shaders/MeshShaders/FbxShader.frag");

	/*	glm::vec4(0.3f, 0.01f, 0.2f, 1),
		glm::vec4(0.9f, 0.3f, 0.4f, 1)*/

	m_pEmitter = new GPUParticleEmitter();
	m_pEmitter->Initialise(
		1000000,
		0.01f, 30.0f,	
		5.0f, 25.0f,
		0.001f, 0.07f,
		glm::vec4(0.0f, 0.3f, 0.9f, 1),
		glm::vec4(0.2f, 1.0f, 1.0f, 1));
		
	//Initialise camera
	InitialiseFlyCamera(10.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	m_starterColour = glm::vec4(0.0f, 0.3f, 0.9f, 1);
	m_endingColour = glm::vec4(0.2f, 1.0f, 1.0f, 1);
	spherePos = glm::vec3(0);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Colour Min", TW_TYPE_COLOR4F, (void*)&m_starterColour);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Colour Max", TW_TYPE_COLOR4F, (void*)&m_endingColour);

	m_pModel = new FBXModel(vec3(0));
	m_pModel->LoadFBX(*m_pMainProgramID, "Data/Models/cube.fbx");
}

// Destroy things 
void Tutorial06::ShutDown()
{
}

// Updates inputs and incr time
void Tutorial06::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

// Draws the stuff to screen
void Tutorial06::Render()
{
	GLApplication::Render();
	/*
	spherePos.x = 5 * sin((float)GetElapsedTime());
	spherePos.z = 5 * cos((float)GetElapsedTime*/

	// Particle Emitter
	m_pEmitter->Render((float)GetDeltaTime(),
		(float)GetElapsedTime(),
		m_pBaseCamera->GetWorldTransform(),
		m_pBaseCamera->GetProjectionView(),
		m_starterColour, m_endingColour,
		spherePos);

	m_pModel->Render();
}