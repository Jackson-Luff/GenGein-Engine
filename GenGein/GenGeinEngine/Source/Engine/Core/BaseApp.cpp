#include <string>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

#include "Engine\Input\InputHandle.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\Renderer\SkyBox.h"
#include "Engine\Cameras\BaseCamera.h"
#include "Engine\Cameras\FlyCamera.h"

#include "BaseApp.h"

using namespace Input;
using glm::vec3;
using glm::vec4;
using glm::mat4;

void error_callback(int a_error, const char* a_description)
{
	// Prints the error to console.
	printf("%i  %s", a_error, a_description);
}

BaseApp::BaseApp() :
m_prevTime(0),
m_deltaTime(0),
m_elapsedTime(0),
m_FPS(0),
m_width(0),
m_height(0),
m_title(0)
{}

BaseApp::BaseApp(const int& a_width, const int& a_height, const char* a_title)
{
	// Apply window width, height and title.
	m_width = a_width;
	m_height = a_height;
	m_title = a_title;
	// Set previous time to null
	m_prevTime = glfwGetTime();
}

BaseApp::~BaseApp()
{
	m_pAntTweakGUI->ShutDown();
	delete m_pSkyBox;
	delete m_pBaseCamera;
}

bool BaseApp::InitialiseGL()
{
	// If approaching error, this will print it out
	glfwSetErrorCallback(error_callback);

	// Check: is GLFWinit safe?
	if(!glfwInit())
		return false;

	// Create Window knowing the init is safe
	m_pWindow = glfwCreateWindow(m_width, m_height, m_title, 0, 0 );

	// Check: is the window safe?
	if(m_pWindow == nullptr) {
		glfwTerminate();
		return false;
	}

	// Now we know the window is safe; apply context.
	glfwMakeContextCurrent(m_pWindow);

	// OpenGL Loader Generator
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}
	
	// Background Colouring
	m_backColour = f32vec4(0.0f, 0.3f, 0.7f, 1.0);
	m_backColour = f32vec4(0,0,0, 1.0);
	// All's a-go. 
	return true;
}

void BaseApp::StartUp()
{
	glEnable(GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);

	// Create a center-point based on input
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.266f, 0.286f, 0.305f, 1);
	printf("Program Shader yet to be initialised.\n\n");

	// Initialise GUI and create a tweaker
	m_pAntTweakGUI->Initialise(m_pWindow, m_width, m_height);
	m_pAntTweakGUI->AddTweaker( "Main Tweaker" );
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug", "DeltaTime", TW_TYPE_DOUBLE, (void*)&m_deltaTime);

	m_pSkyBox = new SkyBox();
	m_pSkyBox->Create(SkyBox::SKY);

	m_elapsedTime = 0;
}

void BaseApp::Update(const double& a_dt)
{
	CalculateTiming();

	DebugControls();

	m_pBaseCamera->Update(a_dt);
}

void BaseApp::DebugControls()
{
	if (Keyboard::isKeyDown(KEY_GRAVE_ACCENT))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (Keyboard::isKeyDown(KEY_1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (Keyboard::isKeyDown(KEY_2))
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (Keyboard::isKeyDown(KEY_P))
		ShaderHandler::ReloadAllPrograms();
}

void BaseApp::CalculateTiming()
{
	// Calculating dt based on glfw time
	const double currTime = glfwGetTime();
	m_deltaTime = currTime - m_prevTime;
	m_prevTime = currTime;

	m_elapsedTime += (float)m_deltaTime;
	m_FPS = 1.0f / m_deltaTime;
}

void BaseApp::RenderSkyBox()
{
	TextureHandler::RenderAllTextures();

	glDepthFunc(GL_LEQUAL);
	glUseProgram(ShaderHandler::GetShader("SkyBox"));
	m_pSkyBox->Render();
	glDepthFunc(GL_LESS);
}

void BaseApp::Render()
{
	// Clear colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ApplyCameraUniformSetup();
}

void BaseApp::InitialiseAppElements()
{
	m_sunPosition = f32vec3(0, 300, 0);
	ApplyCameraUniformSetup();

	// Add a read only FPS output variable to the GUI tweaker specified
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug", "FPS: ", TW_TYPE_DOUBLE, (void*)&m_FPS);

	// Add read only Camera position/speed variables to the GUI tweaker specified
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug",  "X: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetPosition().x);
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug",  "Y: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetPosition().y);
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug",  "Z: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetPosition().z);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Debug", "Speed: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetBaseSpeed());
}

void BaseApp::ApplyCameraUniformSetup()
{
	ShaderHandler::SetUpCameraUniforms(
		m_pBaseCamera->GetProjection(),
		m_pBaseCamera->GetView(),
		m_pBaseCamera->GetWorldTransform());
}

void BaseApp::ApplyLightingSetup(
	const f32vec3& a_ambient,
	const f32vec3& m_sunPosition,
	const float32_t& a_strtLightingHeight)
{
	ShaderHandler::SetUpLightingUniforms(
		a_ambient,
		m_sunPosition,
		a_strtLightingHeight,
		(float32_t)GetElapsedTime());
}

void BaseApp::InitialiseFlyCamera(const float& a_minSpeed,
	const float32_t& a_maxSpeed,
	const float32_t& a_rotationSpeed,
	const f32vec3& a_position,
	const f32vec3& a_lookAt)
{
	//Initialise camera
	FlyCamera* pFlyCam = new FlyCamera(a_minSpeed, a_maxSpeed, a_rotationSpeed);
	pFlyCam->SetPosition(f32vec4(a_position,1));
	pFlyCam->LookAt(a_lookAt);
	pFlyCam->InitialisePerspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 10000.0f);
	pFlyCam->SetInputWindow(m_pWindow);
	m_pBaseCamera = pFlyCam;
}
//
//void BaseApp::InitialisePanZoomCamera(
//	const float32_t& a_moveSpeed,
//	const float32_t& a_rotSpeed,
//	const f32vec3& a_position,
//	const f32vec3& a_lookAt)
//{
//	//Initialise camera
//	PanZoomCamera* pPanCam = new PanZoomCamera(a_moveSpeed, a_rotSpeed);
//	pPanCam->SetPosition(f32vec4(a_position, 1));
//	pPanCam->LookAt(a_lookAt);
//	pPanCam->InitialisePerspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 10000.0f);
//	pPanCam->SetInputWindow(m_pWindow);
//	m_pBaseCamera = pPanCam;
//}

void BaseApp::Run()
{
	/*__________Game Logic Circuit__________
	| GLFWwindow (?)						|
	|   |     							    |
	|	|--|T|--|-> Create Items and Camera.|
	|   |     | 						    |
	|   |     |-> GLFWwindow active (?)	    |
	|   |		   |						|
	|   |		   |-|T|-> Draw items.	    |
	|  |F|		   |						|
	|	|		  |F|						|
	|	|		   |-> Shutdown App.		|
	|	|<---------|						|
	| Terminate							    |
	|_______________________________________*/

	if (!InitialiseGL()) 
		return;
	
	StartUp();
	InitialiseAppElements();

	while(!glfwWindowShouldClose(m_pWindow) 
		&& !Keyboard::isKeyDown(KEY_ESCAPE))
	{
		Update( m_deltaTime );
		Render();
		m_pAntTweakGUI->Draw();

		// Swap Buffers of Window
		glfwSwapBuffers(m_pWindow);
		// Poll Various Events (E.g Keyboard)
		glfwPollEvents();
	}

	ShutDown();
	m_pAntTweakGUI->ShutDown();
	
	ShaderHandler::UnloadAllPrograms();

	// Destory the window and terminate process
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}