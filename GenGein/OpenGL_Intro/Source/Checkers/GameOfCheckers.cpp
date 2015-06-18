#include <gl_core_4_4.h>
#include <glm/glm.hpp>

#include "Engine\Cameras\BaseCamera.h"
#include "CheckersBoard.h"

#include "GameOfCheckers.h"

GameOfCheckers::GameOfCheckers(const int32_t a_width, 
	const int32_t a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

GameOfCheckers::~GameOfCheckers()
{
	delete m_checkerBoard;
}

void GameOfCheckers::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		f32vec3(0,3,4), f32vec3(0));

	m_checkerBoard = new CheckersBoard();
	m_checkerBoard->Initialise();
}

void GameOfCheckers::ShutDown()
{
	m_checkerBoard->Shutdown();
}

void GameOfCheckers::Update(const double_t& a_dt)
{
	BaseApp::Update(a_dt);
	m_checkerBoard->Update(a_dt, 
		m_pBaseCamera->PickAgainstPlane(f32vec4(0,1,0,0.0f)),
		glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1),
		glfwGetKey(m_pWindow, GLFW_KEY_R));	
}

void GameOfCheckers::Render()
{
	BaseApp::Render();
	RenderSkyBox();
	m_checkerBoard->Render(m_pBaseCamera->GetProjectionView());

	ApplyLightingSetup(vec3(0.3),
		vec3(2 * sin(GetElapsedTime()), 2, 2 * cos(GetElapsedTime())),
		0);
}