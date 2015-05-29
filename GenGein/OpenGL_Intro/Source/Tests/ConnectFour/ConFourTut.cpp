#include <gl_core_4_4.h>
#include <glm/glm.hpp>

#include "Tests\ConnectFour\ConnectFour.h"
#include "AIE\Gizmos.h"

#include "Engine\GUI\AntTweak.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Cameras\BaseCamera.h"
#include "ConFourTut.h"

ConFourTut::ConFourTut() {}

ConFourTut::ConFourTut(c_int a_width, c_int a_height, c_pChar a_title)
: BaseApp(a_width, a_height, a_title)
{}

ConFourTut::~ConFourTut()
{
	delete m_ai;
	delete m_game;
}

void ConFourTut::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(20), glm::vec3(0));

	Gizmos::create();

	m_game = new ConnectFour();

	m_ai = new RandomAI();
	
}

void ConFourTut::ShutDown()
{
}

void ConFourTut::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);

	// kepp track of if the mouse has been clicked
	static bool moveMade = false;
	// if the game isn't over...
	if (m_game->getCurrentGameState() == Game::UNKNOWN) {
		// if it is the user's turn (player 1)
		if (m_game->getCurrentPlayer() == Game::PLAYER_ONE) {
			// if the mouse button is down...
			if (glfwGetMouseButton(m_pWindow, 0) == GLFW_PRESS) {
				// if the mouse JUST went down then make a choice
				if (moveMade == false) {
					moveMade = true;
					// get the mouse position within the game grid
					double x = 0, y = 0;
					glfwGetCursorPos(m_pWindow, &x, &y);
					m_pickPosition = m_pBaseCamera->PickAgainstPlane(glm::vec4(0, 1, 0, 0));
					// determine which column was chosen
					int column = (int)((m_pickPosition.z +
						ConnectFour::COLUMNS) / 2);
					// if we clicked in the area for a
					// valid column then perform that action
					if (m_game->isActionValid(column))
						m_game->performAction(column);
				}
			}
			else
				moveMade = false;
		}
		else {
			// it is the opponent's turn (player 2)
			// use the A.I. to make a decision
			m_game->performAction(m_ai->makeDecision(*m_game));
		}
	}
}

void ConFourTut::Render()
{
	BaseApp::Render();
	Gizmos::clear();
	RenderSkyBox();

	m_game->draw();

	Gizmos::draw(m_pBaseCamera->GetProjectionView());
}
