#pragma once
#include <glm\glm.hpp>
#include "Checkers\CheckersBoard.h"

class LogicHandle;

using TURN_SYS = CheckersBoard::TURN_SYS;

////
// Author: Jackson Luff
// Name: CheckersAI
// Type: class (standard)
// Parent: none
// Description:
// * To be clear, this AI compontent
// * is strickly Monte Carlo Search
// * Tree. (MCST) It also competes
// * against the player real-time.
////
class MCTS
{
public:
	MCTS(const int32_t a_playOuts) : 
		m_playOuts(a_playOuts),
		m_turn(TURN_SYS::AI) {}

	virtual ~MCTS() {}

	virtual const int32_t MakeDecision(LogicHandle& a_board);

	inline const TURN_SYS& GetTurn() const
		{ return m_turn; }

private:
	// The number of times it will sim a game
	int32_t m_playOuts;

	TURN_SYS m_turn;
};

