#include <gl_core_4_4.h>
#include <glm/glm.hpp>

#include "Engine\Cameras\BaseCamera.h"
#include "CheckersBoard.h"

#include "GameOfCheckers.h"

GameOfCheckers::GameOfCheckers(c_int a_width, c_int a_height, c_pChar a_title)
: BaseApp(a_width, a_height, a_title)
{}

GameOfCheckers::~GameOfCheckers()
{}

void GameOfCheckers::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(0,10,4), glm::vec3(0,5,-3));

	m_checkerBoard = new CheckersBoard();
	m_checkerBoard->Initialise();
}

void GameOfCheckers::ShutDown()
{
	delete m_checkerBoard;
}

void GameOfCheckers::Update(const double a_dt)
{
	BaseApp::Update(a_dt);
	m_checkerBoard->Update(a_dt, 
		m_pBaseCamera->PickAgainstPlane(glm::vec4(0,1,0,7.32)),
		glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1));	
}

void GameOfCheckers::Render()
{
	BaseApp::Render();
	RenderSkyBox();
	m_checkerBoard->Draw(m_pBaseCamera->GetProjectionView());
}