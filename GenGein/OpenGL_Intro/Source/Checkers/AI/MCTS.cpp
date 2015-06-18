#include <glm\glm.hpp>
#include "Checkers\Core\LogicHandle.h"
#include "MCTS.h"

const int32_t MCTS::MakeDecision(LogicHandle& a_board)
{
	m_turn = TURN_SYS::PLAYER;
	return 0;
}

//
//const int32_t MakeDecisionPseudo()
//{
//	// Create new board
//	// Gen Possible moves for board
//	//int32_t depth = 10;
//	//int32_t bestMoveIndex = -1;
//	//float32_t bestFitness = -999;
//	//float32_t fitness;
//	//int32_t playOuts = 10;
//	//for (int32_t p = 0; p < playOuts; p++)
//	//{
//	//	//nextBoard = new Board
//	//	//RunMove(nextBoard, posMoves[p]);
//	//	fitness = 0;
//	//
//	//	//Play a number of sim games
//	//	for (int32_t s = 0; s < playOuts; s++)
//	//	{
//	//		//tempBoard = nextBoard
//	//
//	//		//TileID = Black
//	//		// TileID == Black? TileID = WHITE
//	//
//	//		for (int32_t d = 0; d < depth; d++)
//	//		{
//	//			//vector<Movement> moves = Get Valid Moves();
//	//
//	//			// size check for = 0
//	//
//	//			// index = rand() % moves.size();
//	//
//	//			//Movement chosenMove = moves[index];
//	//
//	//			//RunMove(tempBoard, chosenMove);
//	//
//	//			//turn == White? turn == Black 
//	//			// else turn == WHITE
//	//		}
//	//
//	//		//fitness += EvaluateFitness(tempBoard);
//	//	}
//	//
//	//	//fitness /= playOuts;
//		//fitness += EvaluateFitness(nextBoard);
//
//	//	if (fitness > bestFitness)
//	//	{
//	//		bestFitness = fitness;
//	//		bestMoveIndex = p;
//	//	}
//	//}
//	//
//	//return possMoves[bestMoveIndex];
//	return 0;
//}
//
//float EvaluateFitness(LogicHandle& a_board)
//{
//	//LogicHandle testBoard = a_board;
//
//	//TileID enemyID = TileID::PLAYER_RED;
//	//if (m_colour == TileID::PLAYER_RED)
//	//	enemyID = TileID::PLAYER_BLACK;
//
//	// get number of my pieces via GetNumberOf(tileID)
//	// get number of their pieces via GetNumberOf(tileID)
//	// get number of total pieces via GetNumberOf(tileID)
//
//	// return (float)myPieces / (float)totalPieces;
//	return 0.0f;
//}