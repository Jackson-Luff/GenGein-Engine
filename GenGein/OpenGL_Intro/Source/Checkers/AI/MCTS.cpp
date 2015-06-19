#include <glm\glm.hpp>
#include "Checkers\Core\LogicHandle.h"
#include "MCTS.h"

using TileID = LogicHandle::TileID;

const bool MCTS::PlayTurn()
{
	PairOfIndex move = MakeDecision();

	//Move based on optimum target
	if (m_pLogic->TryAToB(move.first, move.second, false))
	{
		m_turn = TURN_SYS::PLAYER;
		return true;
	}

	return false;
}

const PairOfIndex MCTS::MakeDecision()
{
	// Copy off a new board
	LogicHandle newBoard = *m_pLogic;

	// Gen possible moves for a colour
	VectorOfMoves possMoves = newBoard.GeneratePossibleMoves(TURN_SYS::AI);

	if (possMoves.size() == 0)
		m_pLogic->TryForGameOver();

	// Variables used to determine optimum playthrough
	int32_t leafNodesSize = possMoves.size();
	int32_t bestMoveIndex = -1;
	int32_t depth = 10;
	float32_t bestWeight = -999;
	float32_t currWeight = 0;
	
	// Selection Step
	for (int32_t l = 0; l < leafNodesSize; l++)
	{
		// Instance another board
		LogicHandle nextBoard = newBoard;
		// Iterate possible playthroughs for selection
		nextBoard.TryAToB(possMoves[l].first, possMoves[l].second, false);
		// Nullify weight per instance of playthrough
		currWeight = 0;

		for (int32_t t = 0; t < m_playOuts; t++)
		{
			LogicHandle tempBoard = nextBoard;

			// Ensures it starts on the other players turn
			TURN_SYS currTurn = TURN_SYS::AI;
			if (currTurn == TURN_SYS::AI)
				currTurn = TURN_SYS::PLAYER;

			//Depth into game (normally until the end)
			for (int32_t d = 0; d < depth; d++)
			{
				// Generate Moves for Player
				VectorOfMoves tempMoves = tempBoard.GeneratePossibleMoves(currTurn);

				if (tempMoves.size() <= 0)
					break;

				// Randomly chosen move to play
				int32_t tempIndex = rand() % tempMoves.size();
				PairOfIndex chosenMove = tempMoves[tempIndex];

				// Play random move
				tempBoard.TryAToB(chosenMove.first, chosenMove.second, false);
				
				// Switch to temp player turn
				if (currTurn == TURN_SYS::PLAYER)
					currTurn = TURN_SYS::AI;
				else
					currTurn = TURN_SYS::PLAYER;
			}
			// Apply ratio based on pieces on board
			currWeight -= EvaluateWeighting(tempBoard);
		}

		// Determine average weight of playthrough
		currWeight /= leafNodesSize;
		currWeight *= EvaluateWeighting(nextBoard);

		// Update the best weighting
		if (currWeight > bestWeight)
		{
			bestWeight = currWeight;
			bestMoveIndex = l;
		}
	}

	// Optimum choice
	return possMoves[bestMoveIndex];
}

const float32_t MCTS::EvaluateWeighting(LogicHandle& a_pLogic)
{
	TURN_SYS oppType = TURN_SYS::PLAYER;
	if (oppType == TURN_SYS::PLAYER)
		oppType = TURN_SYS::AI;

	const int32_t myPieces = a_pLogic.GetNumberOf(m_turn);
	const int32_t theirPieces = a_pLogic.GetNumberOf(oppType);
	float32_t totalPieces = (float32_t)(myPieces + theirPieces);

	return myPieces / totalPieces;
}