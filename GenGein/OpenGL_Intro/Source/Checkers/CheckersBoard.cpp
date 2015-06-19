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
	m_pAI = new MCTS(m_pLogic, 80);

	m_turn = TURN_SYS::PLAYER;
}

void CheckersBoard::Update(const double_t& a_dt, const f32vec3& a_cursPos,
	const int32_t a_isClick, const int32_t a_isRKeyDown)
{
	if (a_isRKeyDown)
		m_pLogic->InitialiseBasicBoard();

	switch (m_turn)
	{
	case CheckersBoard::TURN_SYS::PLAYER:
		if (m_pInput->Update(a_dt, a_cursPos, a_isClick))
			m_turn = TURN_SYS::AI;
		break;
	case CheckersBoard::TURN_SYS::AI:
		if (m_pAI->PlayTurn())
			m_turn = TURN_SYS::PLAYER;
		break;
	}
	
	m_pAudio->Update(a_dt);

	m_pLogic->TryForGameOver();
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