#include <string>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\Renderer\SkyBox.h"

#include "Cameras\BaseCamera.h"
#include "Cameras\FlyCamera.h"

#include "GLApplication.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

void error_callback(c_int a_error, c_charp a_description)
{
	// Prints the error to console.
	printf("%i  %s", a_error, a_description);
}

GLApplication::GLApplication() :
m_prevTime(0),
m_width(0),
m_height(0),
m_title(0)
{}

GLApplication::GLApplication(c_int a_width, c_int a_height, c_charp a_title)
{
	// Apply window width, height and title.
	m_width = a_width;
	m_height = a_height;
	m_title = a_title;
	// Set previous time to null
	m_prevTime = 0;
}

GLApplication::~GLApplication() 
{
	delete m_pSkyBox;
	delete m_pBaseCamera;
}

bool GLApplication::InitialiseGL()
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
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}
	
	// Background Colouring
	m_backColour = vec4(0.0f, 0.3f, 0.7f, 1.0);
	m_backColour = vec4(0,0,0, 1.0);
	// All's a-go. 
	return true;
}

void GLApplication::StartUp()
{
	m_prevTime = 0;
	m_deltaTime = 0;
	m_elapsedTime = 0;
	m_FPS = 0;

	glEnable(GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);

	// Create a center-point based on input
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	printf("Program Shader yet to be initialised.\n\n");

	// Initialise GUI and create a tweaker
	m_pAntTweakGUI->Initialise(m_pWindow, m_width, m_height);
	m_pAntTweakGUI->AddTweaker( "Main Tweaker" );

	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug", "Back Colour", TW_TYPE_COLOR32, (void*)&m_backColour);
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug", "DeltaTime", TW_TYPE_DOUBLE, (void*)&m_deltaTime);

	m_pSkyBox = new SkyBox();
	m_pSkyBox->Create("Data/SkyBox/Faces/Space/");
}

void GLApplication::DebugControls()
{

	if (glfwGetKey(m_pWindow, GLFW_KEY_GRAVE_ACCENT))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(m_pWindow, GLFW_KEY_2))
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS)
		ShaderHandler::ReloadAllPrograms();
}

void GLApplication::CalculateTiming()
{
	// Calculating dt based on glfw time
	m_deltaTime = glfwGetTime() - m_prevTime;
	m_prevTime = glfwGetTime();

	m_elapsedTime += (float)m_deltaTime;
	m_FPS = 1.0f / m_deltaTime;
}

void GLApplication::Render()
{
	// Clear colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ApplyCameraUniformSetup();
	glDepthFunc(GL_LEQUAL);
	m_pSkyBox->Render();
	glDepthFunc(GL_LESS);
	DebugControls();

	CalculateTiming();

	glClearColor(m_backColour.x, m_backColour.y, m_backColour.z, m_backColour.w);
}

void GLApplication::InitialiseAppElements()
{
	ApplyCameraUniformSetup();

	// Add a read only FPS output variable to the GUI tweaker specified
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug", "FPS: ", TW_TYPE_DOUBLE, (void*)&m_FPS);

	// Add read only Camera position/speed variables to the GUI tweaker specified
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug",  "X: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetPosition().x);
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug",  "Y: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetPosition().y);
	m_pAntTweakGUI->AddVarRO("Main Tweaker", "Debug",  "Z: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetPosition().z);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Debug", "Speed: ", TW_TYPE_FLOAT, (void*)&m_pBaseCamera->GetBaseSpeed());
}

void GLApplication::ApplyCameraUniformSetup()
{
	ShaderHandler::SetUpCameraUniforms(
		m_pBaseCamera->GetProjection(),
		m_pBaseCamera->GetView(),
		m_pBaseCamera->GetWorldTransform(),
		m_pBaseCamera->GetPosition(),
		(float)GetElapsedTime());
}

void GLApplication::ApplyLightingSetup(
	const vec4& a_ambient,
	const vec4& a_diffuse,
	const vec4& a_specular,
	const mat4& a_shadowMatrix)
{
	ShaderHandler::SetUpLightingUniforms(
		a_ambient,
		a_diffuse,
		a_specular,
		a_shadowMatrix);
}

void GLApplication::InitialiseFlyCamera(c_float a_minSpeed,
	c_float a_maxSpeed, c_float a_rotationSpeed,
	glm::vec3 a_position, glm::vec3 a_lookAt)
{
	//Initialise camera
	FlyCamera* pFlyCam = new FlyCamera(a_minSpeed, a_maxSpeed, a_rotationSpeed);
	pFlyCam->SetPosition(vec4(a_position,1));
	pFlyCam->LookAt(a_lookAt);
	pFlyCam->InitialisePerspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 10000.0f);
	pFlyCam->SetInputWindow(m_pWindow);
	m_pBaseCamera = pFlyCam;
}

void GLApplication::Run()
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

	if (!InitialiseGL()) { return; };
	
	StartUp();
	InitialiseAppElements();

	while(!glfwWindowShouldClose(m_pWindow) 
		&& glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		Update( GetDeltaTime() );
		Render();
		m_pAntTweakGUI->Draw();

		// Swap Buffers of Window
		glfwSwapBuffers(m_pWindow);
		// Poll Various Events (E.g Keyboard)
		glfwPollEvents();
	}

	ShutDown();

	// Destory the window and terminate process
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}