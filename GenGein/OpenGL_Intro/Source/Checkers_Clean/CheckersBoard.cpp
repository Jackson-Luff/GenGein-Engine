#include "CheckersLogic.h"
#include "CheckersVisual.h"
#include "CheckersInput.h"

#include "CheckersBoard.h"

CheckersBoard::~CheckersBoard()
{
	delete m_boardLogic;
	delete m_boardVisual;
	delete m_boardInput;
}

void CheckersBoard::Initialise()
{
	m_boardLogic = new CheckersLogic();
	m_boardLogic->InitialiseBasicBoard();

	m_boardVisual = new CheckersVisual();
	m_boardVisual->Initialise(m_boardLogic);

	m_boardInput = new CheckersInput();
	m_boardInput->Initialise(m_boardLogic, m_boardVisual);
}

void CheckersBoard::Update(const double_t& a_dt, const f32vec3& a_cursPos, const int a_isClick)
{
	m_boardInput->Update(a_dt, a_cursPos, a_isClick);
}

void CheckersBoard::Render(const glm::mat4& a_camProjView)
{
	m_boardVisual->Render(a_camProjView);
}