#include <gl_core_4_4.h>
#include <glm/glm.hpp>

#include "Engine\Cameras\BaseCamera.h"
#include "CheckerBoard.h"

#include "CheckersGame.h"

CheckersGame::CheckersGame(c_int a_width, c_int a_height, c_pChar a_title)
: BaseApp(a_width, a_height, a_title)
{}

CheckersGame::~CheckersGame()
{
}

void CheckersGame::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(10), glm::vec3(0));

	m_checkerBoard = new CheckerBoard();
	m_checkerBoard->Initialise();
}

void CheckersGame::ShutDown()
{
}

void CheckersGame::Update(const double a_dt)
{
	BaseApp::Update(a_dt);
}

void CheckersGame::Render()
{
	BaseApp::Render();

	m_checkerBoard->Draw(m_pBaseCamera->GetProjectionView());

	RenderSkyBox();
}
