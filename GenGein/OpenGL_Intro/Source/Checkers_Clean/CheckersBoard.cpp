#include "CheckersLogic.h"
#include "CheckersVisual.h"
#include "CheckersInput.h"
#include "CheckersAudio.h"

#include "CheckersBoard.h"

CheckersBoard::~CheckersBoard()
{
	delete m_pLogic;
	delete m_pVisual;
	delete m_pInput;
}

void CheckersBoard::Initialise()
{
	m_pLogic = new CheckersLogic();
	m_pLogic->InitialiseBasicBoard();

	m_pVisual = new CheckersVisual();
	m_pVisual->Initialise(m_pLogic);

	m_pInput = new CheckersInput();
	m_pInput->Initialise(m_pLogic, m_pVisual);

	//m_pAudio = new CheckersAudio();
	//m_pAudio->Initialise();
}

void CheckersBoard::Update(const double_t& a_dt, const f32vec3& a_cursPos, const int a_isClick)
{
	m_pInput->Update(a_dt, a_cursPos, a_isClick);
	//m_pAudio->Update(a_dt);
}

void CheckersBoard::Render(const glm::mat4& a_camProjView)
{
	m_pVisual->Render(a_camProjView);
}