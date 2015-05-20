#include <AIE\Gizmos.h> //REMOVE

#include <gl_core_4_4.h>
#include <glm\glm.hpp>

#include "Engine\Core\ShaderHandler.h"
#include "Engine\Cameras\BaseCamera.h"
#include "Engine\Objects\FBXModel.h"
#include "Checkers\Player.h"
#include "Checkers.h"

Checkers::Checkers(){}

Checkers::Checkers(c_int a_width, c_int a_height, c_pChar a_title)
	: BaseApp(a_width, a_height, a_title)
{

}

Checkers::~Checkers()
{}

void Checkers::GenCheckerBoardData(const float a_boardSize, const float a_tileCount)
{
	float tileSize = a_boardSize / a_tileCount;
	float halfSize = a_boardSize / 2.0f;

	m_tileCount = a_tileCount;
	m_tileSize = tileSize;
	m_boardSize = a_boardSize;

	//Verts
	verts[0] = { glm::vec3(-halfSize, 0, halfSize), glm::vec2(0, 1) };
	verts[1] = { glm::vec3(halfSize, 0, halfSize), glm::vec2(1, 1) };
	verts[2] = { glm::vec3(halfSize, 0, -halfSize), glm::vec2(1, 0) };
	verts[3] = { glm::vec3(-halfSize, 0, -halfSize), glm::vec2(0, 0) };

	uint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	uint m_VBO, m_IBO;

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 
		sizeof(CHECKERS_DATA::BoardData), verts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		sizeof(CHECKERS_DATA::BoardData),
		(void*)offsetof(CHECKERS_DATA::BoardData, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(CHECKERS_DATA::BoardData),
		(void*)offsetof(CHECKERS_DATA::BoardData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint), indices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);

	for (int r = 0; r < a_tileCount; r++)
	{
		for (int c = 0; c < a_tileCount; c++)
		{
			glm::vec3 pos = vec3(r * tileSize, 0, c * tileSize);
			pos.z -= halfSize;
			pos.z += tileSize / 2.0f;
			pos.x -= tileSize * 3 + tileSize / 2.0f;

			if ((r+c) % 2 == 0)
				m_possiblePositions.push_back(pos);

			//use mod to find white/black based on index on x,y
		}
	}

}

// Initialise things
void Checkers::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		vec3(-10, 9, 0), vec3(0));

	Gizmos::create();

	ShaderHandler::LoadShaderProgram("CheckerBoard",
		"Data/Shaders/Checkers/Assets/CheckerBoard.vert",
		"Data/Shaders/Checkers/Assets/CheckerBoard.frag");
	m_checkerProgID = &ShaderHandler::GetShader("CheckerBoard");


	GenCheckerBoardData( 10.0f, 8.0f);

	m_playerOne = new Player(CHECKERS_DATA::PLAYER_01);
	m_playerOne->Initialise();
	m_playerOne->ApplyPositions(m_tileSize, m_possiblePositions);

	m_playerTwo = new Player(CHECKERS_DATA::PLAYER_02);
	m_playerTwo->Initialise();
	m_playerTwo->ApplyPositions(m_tileSize, m_possiblePositions);
}

// Destroy things
void Checkers::ShutDown()
{
	Gizmos::destroy();
}

// Update loop
void Checkers::Update(const double a_dt)
{
	BaseApp::Update(a_dt);

	double x, y;
	glfwGetCursorPos(m_pWindow, &x, &y);
	
	m_playerOne->Update(a_dt,
		m_pBaseCamera->PickAgainstPlane(
		(float)x, (float)y, glm::vec4(0, 1, 0, 0)),
		glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1));

	m_playerTwo->Update(a_dt,
		m_pBaseCamera->PickAgainstPlane(
		(float)x, (float)y, glm::vec4(0, 1, 0, 0)),
		glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1));
}

// Render things to screen
void Checkers::Render()
{
	BaseApp::Render();
	RenderSkyBox();

	glUseProgram(*m_checkerProgID);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	m_playerOne->Draw(m_pBaseCamera->GetProjectionView(), m_tileSize);
	m_playerTwo->Draw(m_pBaseCamera->GetProjectionView(), m_tileSize);
	//m_boardBase->Render();
}