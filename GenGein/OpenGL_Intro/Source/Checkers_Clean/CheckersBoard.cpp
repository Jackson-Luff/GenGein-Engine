#include "CheckersLogic.h"
#include "CheckersVisual.h"
#include "CheckersInput.h"

#include "CheckersBoard.h"

CheckersBoard::CheckersBoard()
{}

CheckersBoard::~CheckersBoard()
{}

void CheckersBoard::Initialise()
{
	m_boardLogic = new CheckersLogic();
	m_boardLogic->InitialiseBasicBoard();

	m_boardVisual = new CheckersVisual();
	m_boardVisual->Initialise(m_boardLogic);

	m_boardInput = new CheckersInput();
	m_boardInput->Initialise(m_boardLogic, m_boardVisual);
}

void CheckersBoard::Update(c_dub a_dt,
	c_vec3 a_cursPos, int a_isClick)
{
	mousePos = a_cursPos;
	m_boardInput->Update(a_dt, a_cursPos, a_isClick);
}

void CheckersBoard::Draw(const glm::mat4& a_camProjView)
{
	m_boardVisual->Draw(a_camProjView);
	m_boardVisual->RenderAtMousePosition(mousePos);
}