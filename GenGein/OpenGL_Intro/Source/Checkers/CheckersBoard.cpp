#include "Core\LogicHandle.h"
#include "Core\VisualHandle.h"
#include "Core\InputHandle.h"
#include "Core\AudioHandle.h"
#include "AI\MCTS.h"

#include "CheckersBoard.h"

CheckersBoard::~CheckersBoard()
{
	delete m_pLogic;
	delete m_pVisual;
	delete m_pInput;
	delete m_pAudio;
}

void CheckersBoard::Initialise()
{
	m_pLogic = new LogicHandle();
	m_pLogic->InitialiseBasicBoard();

	m_pVisual = new VisualHandle();
	m_pVisual->Initialise(m_pLogic);

	m_pAudio = new AudioHandle();
	m_pAudio->Initialise();

	m_pInput = new InputHandle();
	m_pInput->Initialise(m_pLogic, m_pVisual, m_pAudio);

	//MonteCarlo playouts: 80 
	m_pAI = new MCTS(80);

	m_turn = TURN_SYS::PLAYER;
}

void CheckersBoard::Update(const double_t& a_dt, const f32vec3& a_cursPos,
	const int32_t a_isClick, const int32_t a_isRKeyDown)
{
	if (a_isRKeyDown)
		m_pLogic->InitialiseBasicBoard();

	if (m_turn == m_pInput->GetTurn())
		m_pInput->Update(a_dt, a_cursPos, a_isClick);
	else if (m_turn == m_pAI->GetTurn())
		m_pAI->MakeDecision(*m_pLogic);

	if (m_pAI->GetTurn() == TURN_SYS::PLAYER)
		m_turn = TURN_SYS::PLAYER;

	if (m_pInput->GetTurn() == TURN_SYS::AI)
		m_turn = TURN_SYS::AI;

	m_pAudio->Update(a_dt);
}

void CheckersBoard::Render(const glm::f32mat4& a_camProjView)
{
	m_pVisual->Render(a_camProjView);
}

void CheckersBoard::Shutdown()
{
	m_pVisual->Shutdown();
	m_pAudio->Shutdown();
}