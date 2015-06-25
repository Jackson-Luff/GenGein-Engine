#pragma once
#include <glm\glm.hpp>
#include "Extras\Checkers\CheckersBoard.h"

class LogicHandle;

using TURN_SYS = CheckersBoard::TURN_SYS;
using PairOfIndex = std::pair<i32vec2, i32vec2>;
using VectorOfMoves = std::vector<PairOfIndex>;

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
	// Constructor
	MCTS(LogicHandle* a_pBoard, const int32_t a_playOuts) :
		m_pLogic(a_pBoard),
		m_playOuts(a_playOuts),
		m_turn(TURN_SYS::AI) 
	{}

	// Deconstructor
	virtual ~MCTS() {}

	// Plays AI turn
	const bool PlayTurn();

	// Returns who's turn it is
	inline const TURN_SYS& GetTurn() const
		{ return m_turn; }

private:
	// Make AI decision
	const PairOfIndex MakeDecision();
	// Evaluate the health of move
	const float32_t EvaluateWeighting(LogicHandle& a_pLogic);

	// Board reference (non-&)
	LogicHandle* m_pLogic;
	// Num' of games to sim'
	int32_t m_playOuts;
	// Who's turn it is
	TURN_SYS m_turn;
};

