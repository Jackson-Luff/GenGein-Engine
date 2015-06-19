#include <iostream>	
#include "LogicHandle.h"

using MoveDirection = LogicHandle::MoveDirection;
using JumpDirection = LogicHandle::JumpDirection;
using TileID = LogicHandle::TileID;

LogicHandle::LogicHandle()
: m_indexListOfPosMoves(std::vector<i32vec2>(0)), 
  m_forceJump(false)
{}

const void LogicHandle::InitialiseBasicBoard()
{
	bool white = true;
	for (uint32_t r = 0; r < m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_dimCount; c++)
		{
			if (!white && c < (m_dimCount / 2) - 1)
				m_boardIDs[r*m_dimCount + c] = TileID::PLAYER_RED_KING;
			else if (!white && c < (m_dimCount / 2) + 1)
				m_boardIDs[r*m_dimCount + c] = TileID::BLANK_RED;
			else if (!white && c < m_dimCount)
				m_boardIDs[r*m_dimCount + c] = TileID::PLAYER_BLACK;
			else
				m_boardIDs[r*m_dimCount + c] = TileID::BLANK_BLACK;
			white = !white;
		}
		white = !white;
	}
}

const TileID& LogicHandle::GetIDAt(const i32vec2& a_cIndex)
{
	if (!IsWithinBoard(a_cIndex))
		return m_nullTile;

	return m_boardIDs[(a_cIndex.x * m_dimCount) + a_cIndex.y];
}

const bool LogicHandle::SetPieceAt(const i32vec2& a_cIndex, const TileID& a_type)
{
	if (!IsWithinBoard(a_cIndex))
		return false;

	if (GetIDAt(a_cIndex) == TileID::BLANK_BLACK)
	{
		printf("Cannot Set Piece of Black (invalid type).");
		return false;
	}

	m_boardIDs[(a_cIndex.x * m_dimCount) + a_cIndex.y] = a_type;
	return true;
}

const bool LogicHandle::TryAToB(const i32vec2& a_cIndex, const i32vec2& a_tIndex, bool a_checkMoveList)
{
	if (a_checkMoveList)
	{
		// Is move on possible move list (may not need this)
		if (!IsInPossibleMoveList(a_tIndex))
			return false;
	}

	// These are just triggers. Remember we
	// already checked if they're valid and
	// what not in the generation of them. :)
	
	// determines direction of movement:
	// |A|_|B|  C - A = -1, +1 = MOVE_TL | C - A*2 = -2, -2 = JUMP_TL
	// |_|C|_|  C - B = +1, +1 = MOVE_TR | C - B*2 = -2, -2 = JUMP_TR
	// |D|_|E|  C - E = -1, -1 = MOVE_BR | C - E*2 = -2, -2 = JUMP_BR
	//			C - D = +1, -1 = MOVE_BL | C - D*2 = -2, -2 = JUMP_BL
	// =================================
	i32vec2 dir = a_tIndex - a_cIndex;
	TileID curID = GetIDAt(a_cIndex);

	if (abs(dir) == i32vec2(1))
	{
		// if they try to move
		// but they can jump, then
		// they have to jump no matter what.
		if (m_forceJump)
			return false;

		// It's a move!
		if (a_tIndex.y == m_dimCount-1 && curID == TileID::PLAYER_RED)
			SetPieceAt(a_tIndex, TileID::PLAYER_RED_KING);//red king
		else if (a_tIndex.y == 0 && curID == TileID::PLAYER_BLACK)
			SetPieceAt(a_tIndex, TileID::PLAYER_BLACK_KING);//black king
		else
			SetPieceAt(a_tIndex, curID);

		SetPieceAt(a_cIndex, TileID::BLANK_RED);
	}
	else if (abs(dir) == i32vec2(2))
	{
		// It's a jump!
		i32vec2 iIndex = a_cIndex + dir / 2;

		if (a_tIndex.y == m_dimCount - 1 && curID == TileID::PLAYER_RED)
			SetPieceAt(a_tIndex, TileID::PLAYER_RED_KING);//red king
		else if (a_tIndex.y == 0 && curID == TileID::PLAYER_BLACK)
			SetPieceAt(a_tIndex, TileID::PLAYER_BLACK_KING);//black king
		else
			SetPieceAt(a_tIndex, curID);

		SetPieceAt(iIndex, TileID::BLANK_RED);
		SetPieceAt(a_cIndex, TileID::BLANK_RED);
	}

	return true;
}

const bool LogicHandle::TryMove(const i32vec2& a_cIndex, const MoveDirection& a_type)
{
	i32vec2 tIndex = MoveDirTo2DIndex(a_cIndex, a_type);

	// Is move on possible move list (may not need this)
	if (!IsInPossibleMoveList(tIndex))
		return false;

	SetPieceAt(tIndex, GetIDAt(a_cIndex));
	SetPieceAt(a_cIndex, TileID::BLANK_RED);
	return true;
}

const bool LogicHandle::TryJump(const i32vec2& a_cIndex, const JumpDirection& a_type)
{
	// Target index
	i32vec2 tIndex = JumpDirTo2DIndex(a_cIndex, a_type);

	if (!IsValidJump(a_cIndex, a_type))
		return false;

	//Collect intercepted piece
	SetPieceAt(tIndex, GetIDAt(a_cIndex));
	//SetPieceAt(iIndex,	 TileID::BLANK_RED);
	SetPieceAt(a_cIndex, TileID::BLANK_RED);

	return true;
}

const bool LogicHandle::TryForGameOver()
{
	int32_t redCount = 0, blackCount = 0;

	for (int32 r = 0; r < m_dimCount; r++)
	{
		for (int32 c = 0; c < m_dimCount; c++)
		{
			TileID cInd = GetIDAt(i32vec2(r, c));

			if (cInd == TileID::BLANK_BLACK ||
				cInd == TileID::BLANK_RED)
				continue;

			if (cInd == TileID::PLAYER_RED || cInd == TileID::PLAYER_RED_KING)
				redCount++;
			else if (cInd == TileID::PLAYER_BLACK || cInd == TileID::PLAYER_BLACK_KING)
				blackCount++;
		}
	}

	if (redCount <= 0 || blackCount <= 0)
	{
		if (redCount > blackCount)
			printf("Game over, RED WINS!");
		else if (redCount > blackCount)
			printf("Game over, BLACK WINS!");
		else
			printf("Game over, DRAW!");
		return true;
	}
		

	return false;
}

const void LogicHandle::GeneratePossibleMoves(const i32vec2& a_cIndex)
{
	if (GetIDAt(a_cIndex) == TileID::BLANK_BLACK || GetIDAt(a_cIndex) == TileID::BLANK_RED)
		return;

	m_forceJump = IsJumpAvaliabeAtAll(TURN_SYS::PLAYER);

	ClearPossibleMovesList();

	TileID ID = GetIDAt(a_cIndex);

	// Red pieces can't move back, thus move up the board 
	if (ID == TileID::PLAYER_RED)
	{
		PerformSurroundingChecksForRed(m_indexListOfPosMoves, a_cIndex);
	} // Black pieces can't move forward, thus move down the board 
	else if (ID == TileID::PLAYER_BLACK)
	{
		PerformSurroundingChecksForBlack(m_indexListOfPosMoves, a_cIndex);
	} // King has all authorisation, and thus every move is possible.
	else if (ID == TileID::PLAYER_RED_KING || ID == TileID::PLAYER_BLACK_KING)
	{
		PerformSurroundingChecksForKings(m_indexListOfPosMoves, a_cIndex);
	}

	if (m_indexListOfPosMoves.size() == 0)
		printf("No valid moves at index; x:%i y:%i \n", a_cIndex.x, a_cIndex.y);
}

const std::vector<std::pair<i32vec2, i32vec2>> LogicHandle::GeneratePossibleMoves(const TURN_SYS a_type)
{
	ClearPossibleMovesList();

	m_forceJump = IsJumpAvaliabeAtAll(a_type);

	std::vector<std::pair<i32vec2, i32vec2>> outList;

	for (int32_t r = 0; r < m_dimCount; r++)
	{
		for (int32_t c = 0; c < m_dimCount; c++)
		{
			i32vec2 cInd = i32vec2(r, c);
			TileID cID = GetIDAt(cInd);

			if (cID == TileID::BLANK_BLACK ||
				cID == TileID::BLANK_RED)
				continue;

			// Red pieces can't move back, thus move up the board 
			if (a_type == TURN_SYS::PLAYER)
			{
				if (cID == TileID::PLAYER_RED)
				{
					if (IsValidMove(cInd, MoveDirection::MOVE_TL))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TL) });
					if (IsValidMove(cInd, MoveDirection::MOVE_TR))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TR) });

					if (IsValidJump(cInd, JumpDirection::JUMP_TL))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TL) });
					if (IsValidJump(cInd, JumpDirection::JUMP_TR))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TR) });
				}
				else if (cID == TileID::PLAYER_RED_KING)
				{
					if (IsValidMove(cInd, MoveDirection::MOVE_TL))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TL) });
					if (IsValidMove(cInd, MoveDirection::MOVE_TR))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TR) });
					if (IsValidMove(cInd, MoveDirection::MOVE_BR))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BR) });
					if (IsValidMove(cInd, MoveDirection::MOVE_BL))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BL) });

					if (IsValidJump(cInd, JumpDirection::JUMP_TL))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TL) });
					if (IsValidJump(cInd, JumpDirection::JUMP_TR))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TR) });
					if (IsValidJump(cInd, JumpDirection::JUMP_BR))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BR) });
					if (IsValidJump(cInd, JumpDirection::JUMP_BL))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BL) });
				}
			}

			// Black pieces can't move forward, thus move down the board 
			if (a_type == TURN_SYS::AI)
			{
				if ((GetIDAt(cInd) == TileID::PLAYER_BLACK))
				{
					if (IsValidMove(cInd, MoveDirection::MOVE_BR))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BR) });
					if (IsValidMove(cInd, MoveDirection::MOVE_BL))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BL) });

					if (IsValidJump(cInd, JumpDirection::JUMP_BR))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BR) });
					if (IsValidJump(cInd, JumpDirection::JUMP_BL))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BL) });
				}
				else if (GetIDAt(cInd) == TileID::PLAYER_BLACK_KING)
				{
					if (IsValidMove(cInd, MoveDirection::MOVE_TL))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TL) });
					if (IsValidMove(cInd, MoveDirection::MOVE_TR))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TR) });
					if (IsValidMove(cInd, MoveDirection::MOVE_BR))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BR) });
					if (IsValidMove(cInd, MoveDirection::MOVE_BL))
						outList.push_back({ cInd, MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BL) });

					if (IsValidJump(cInd, JumpDirection::JUMP_TL))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TL) });
					if (IsValidJump(cInd, JumpDirection::JUMP_TR))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TR) });
					if (IsValidJump(cInd, JumpDirection::JUMP_BR))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BR) });
					if (IsValidJump(cInd, JumpDirection::JUMP_BL))
						outList.push_back({ cInd, JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BL) });
				}
			}
		}
	}

	if (outList.size() == 0)
		printf("No valid moves for TURN_SYS type; %i \n", a_type);

	return outList;
}

const void LogicHandle::ClearPossibleMovesList()
{
	if (m_indexListOfPosMoves.size() > 0)
	{
		printf("Warning: Clearing possible move list.\n");
		m_forceJump = false;
		m_indexListOfPosMoves.clear();
	}
}

const bool LogicHandle::IsInPossibleMoveList(const i32vec2& a_tIndex)
{
	if (std::find(m_indexListOfPosMoves.begin(),
		m_indexListOfPosMoves.end(), a_tIndex) != m_indexListOfPosMoves.end())
		return true;

	return false;
}

const int32_t LogicHandle::GetNumberOf(const TURN_SYS a_whosTurnIsIt)
{
	int numFound = 0;

	for (auto id : m_boardIDs)
	{
		if (id == TileID::BLANK_BLACK ||
			id == TileID::BLANK_RED)
			continue;

		if (a_whosTurnIsIt == TURN_SYS::PLAYER &&
			(id == TileID::PLAYER_RED ||
			 id == TileID::PLAYER_RED_KING))
			numFound++;
		else if (a_whosTurnIsIt == TURN_SYS::AI &&
			(id == TileID::PLAYER_BLACK ||
			 id == TileID::PLAYER_BLACK_KING))
			numFound++;
	}

	if (numFound <= 0)
		printf("Could not find pieces. (GetNumberOf())\n");

	return numFound;
}

const i32vec2 LogicHandle::IsJumpAvaliabeAt(const i32vec2 a_cIndex)
{
	if (IsValidJump(a_cIndex, JUMP_TL))
		return JumpDirTo2DIndex(a_cIndex, JUMP_TL);
	else if (IsValidJump(a_cIndex, JUMP_TR))
		return JumpDirTo2DIndex(a_cIndex, JUMP_TR);
	else if (IsValidJump(a_cIndex, JUMP_BR))
		return JumpDirTo2DIndex(a_cIndex, JUMP_BR);
	else if (IsValidJump(a_cIndex, JUMP_BL))
		return JumpDirTo2DIndex(a_cIndex, JUMP_BL);

	return i32vec2(-1,-1);
}

// Private funcs:

const bool LogicHandle::IsJumpAvaliabeAtAll(const TURN_SYS a_whoAmI)
{
	for (uint32_t r = 0; r < m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_dimCount; c++)
		{
			TileID currID = GetIDAt(i32vec2(r, c));

			if (currID == TileID::BLANK_BLACK ||
				currID == TileID::BLANK_RED)
				continue;

			TileID lowerType;
			TileID kingType;

			if (a_whoAmI == TURN_SYS::AI)
			{
				lowerType = TileID::PLAYER_BLACK;
				kingType = TileID::PLAYER_BLACK_KING;
			}
			else if (a_whoAmI == TURN_SYS::PLAYER)
			{
				lowerType = TileID::PLAYER_RED;
				kingType = TileID::PLAYER_RED_KING;
			}

			if (currID == kingType)
			{
				if (IsValidJump(i32vec2(r, c), JUMP_TL))
					return true;
				else if (IsValidJump(i32vec2(r, c), JUMP_TR))
					return true;
				else if (IsValidJump(i32vec2(r, c), JUMP_BR))
					return true;
				else if (IsValidJump(i32vec2(r, c), JUMP_BL))
					return true;
			}
			else if (currID == lowerType)
			{
				if (lowerType == TileID::PLAYER_RED)
				{
					if (IsValidJump(i32vec2(r, c), JUMP_TL))
						return true;
					else if (IsValidJump(i32vec2(r, c), JUMP_TR))
						return true;
				}
				else if (lowerType == TileID::PLAYER_BLACK)
				{
					if (IsValidJump(i32vec2(r, c), JUMP_BL))
						return true;
					else if (IsValidJump(i32vec2(r, c), JUMP_BR))
						return true;
				}
			}
		}
	}
	return false;
}

const void LogicHandle::PerformSurroundingChecksForRed(std::vector<i32vec2>& a_indexs, const i32vec2 a_cIndex)
{
	// Standard Moves
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TL));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TR));

	// Standard Jumps
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_TL))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TL));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_TR))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TR));

	// Crazy Jump Origins
	//i32vec2 doubleJump_TL = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TL);
	//i32vec2 doubleJump_TR = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TR);
	//i32vec2 tripleJump_TL = JumpDirTo2DIndex(doubleJump_TL, JumpDirection::JUMP_TL);
	//i32vec2 tripleJump_TR = JumpDirTo2DIndex(doubleJump_TR, JumpDirection::JUMP_TR);
	//i32vec2 quadJump_TL = JumpDirTo2DIndex(tripleJump_TL, JumpDirection::JUMP_TL);
	//i32vec2 quadJump_TR = JumpDirTo2DIndex(tripleJump_TR, JumpDirection::JUMP_TR);
	//
	//// Double Jumps
	//if (IsValidJump(doubleJump_TL, JumpDirection::JUMP_TL))
	//	a_indexs.push_back(tripleJump_TL);
	//
	//if (IsValidJump(doubleJump_TR, JumpDirection::JUMP_TR))
	//	a_indexs.push_back(tripleJump_TR);
	//
	//// Triple Jumps
	//if (IsValidJump(tripleJump_TL, JumpDirection::JUMP_TL))
	//	a_indexs.push_back(quadJump_TL);
	//
	//if (IsValidJump(tripleJump_TR, JumpDirection::JUMP_TR))
	//	a_indexs.push_back(quadJump_TR);
}

const void LogicHandle::PerformSurroundingChecksForBlack(std::vector<i32vec2>& a_indexs, const i32vec2 a_cIndex)
{
	// Standard Moves
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BR));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BL));
	// Standard Jumps
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_BR))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BR));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_BL))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BL));
	// Crazy Jump Origins
	//i32vec2 doubleJump_BL = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BL);
	//i32vec2 doubleJump_BR = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BR);
	//i32vec2 tripleJump_BL = JumpDirTo2DIndex(doubleJump_BL, JumpDirection::JUMP_BL);
	//i32vec2 tripleJump_BR = JumpDirTo2DIndex(doubleJump_BR, JumpDirection::JUMP_BR);
	//i32vec2 quadJump_BL = JumpDirTo2DIndex(tripleJump_BL, JumpDirection::JUMP_BL);
	//i32vec2 quadJump_BR = JumpDirTo2DIndex(tripleJump_BR, JumpDirection::JUMP_BR);
	//
	//// Double Jumps
	//if (IsValidJump(doubleJump_BL, JumpDirection::JUMP_BL))
	//	a_indexs.push_back(tripleJump_BL);
	//
	//if (IsValidJump(doubleJump_BR, JumpDirection::JUMP_BR))
	//	a_indexs.push_back(tripleJump_BR);
	//
	//// Triple Jumps
	//if (IsValidJump(tripleJump_BL, JumpDirection::JUMP_BL))
	//	a_indexs.push_back(quadJump_BL);
	//
	//if (IsValidJump(tripleJump_BR, JumpDirection::JUMP_BR))
	//	a_indexs.push_back(quadJump_BR);
}

const void LogicHandle::PerformSurroundingChecksForKings(std::vector<i32vec2>& a_indexs, const i32vec2 a_cIndex)
{
	//Moves
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TL));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TR));

	if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BR));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
		a_indexs.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BL));

	//Jumps
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_TL))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TL));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_TR))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TR));

	if (IsValidJump(a_cIndex, JumpDirection::JUMP_BR))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BR));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_BL))
		a_indexs.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BL));
	// Crazy Jump Origins
	//i32vec2 doubleJump_TL = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TL);
	//i32vec2 doubleJump_TR = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TR);
	//i32vec2 doubleJump_BL = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BL);
	//i32vec2 doubleJump_BR = JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BR);
	//
	//i32vec2 tripleJump_TL = JumpDirTo2DIndex(doubleJump_TL, JumpDirection::JUMP_TL);
	//i32vec2 tripleJump_TR = JumpDirTo2DIndex(doubleJump_TR, JumpDirection::JUMP_TR);
	//i32vec2 tripleJump_BL = JumpDirTo2DIndex(doubleJump_BL, JumpDirection::JUMP_BL);
	//i32vec2 tripleJump_BR = JumpDirTo2DIndex(doubleJump_BR, JumpDirection::JUMP_BR);
	//
	//i32vec2 quadJump_TL = JumpDirTo2DIndex(tripleJump_TL, JumpDirection::JUMP_TL);
	//i32vec2 quadJump_TR = JumpDirTo2DIndex(tripleJump_TR, JumpDirection::JUMP_TR);
	//i32vec2 quadJump_BL = JumpDirTo2DIndex(tripleJump_BL, JumpDirection::JUMP_BL);
	//i32vec2 quadJump_BR = JumpDirTo2DIndex(tripleJump_BR, JumpDirection::JUMP_BR);
	//
	//// Double Jumps
	//if (IsValidJump(doubleJump_BL, JumpDirection::JUMP_BL))
	//	a_indexs.push_back(tripleJump_BL);
	//if (IsValidJump(doubleJump_BR, JumpDirection::JUMP_BR))
	//	a_indexs.push_back(tripleJump_BR);
	//if (IsValidJump(doubleJump_TL, JumpDirection::JUMP_TL))
	//	a_indexs.push_back(tripleJump_TL);
	//if (IsValidJump(doubleJump_TR, JumpDirection::JUMP_TR))
	//	a_indexs.push_back(tripleJump_TR);
	//
	//// Triple Jumps
	//if (IsValidJump(tripleJump_TL, JumpDirection::JUMP_TL))
	//	a_indexs.push_back(quadJump_TL);
	//if (IsValidJump(tripleJump_TR, JumpDirection::JUMP_TR))
	//	a_indexs.push_back(quadJump_TR);
	//if (IsValidJump(tripleJump_BL, JumpDirection::JUMP_BL))
	//	a_indexs.push_back(quadJump_BL);
	//if (IsValidJump(tripleJump_BR, JumpDirection::JUMP_BR))
	//	a_indexs.push_back(quadJump_BR);
}

const i32vec2 LogicHandle::MoveDirTo2DIndex(const i32vec2& a_cIndex, const MoveDirection& a_type)
{
	// Logic example:
	// currIndex = (2,2);
	// m_dimCount = 8;
	// a_type = MOVE_TR = m_dimCount + 1;
	// => ((row * m_dimCount) + col) + a_type = 2Dto1Dindex
	// => 2Dto1Dindex = ((2 * 8) + 2) + (8+1) = 27
	// => targetRow = 2Dto1Dindex % m_dimCount = 27%8 = 3
	// => targetCol = 2Dto1Dindex / m_dimCount = 27/8 = 3
	// #CORRECT ^_^
	int32_t indexIn1D = ((a_cIndex.y * m_dimCount) + a_cIndex.x) + a_type;
	return i32vec2(floor(indexIn1D % m_dimCount), floor(indexIn1D / m_dimCount));
}

const i32vec2 LogicHandle::JumpDirTo2DIndex(const i32vec2& a_cIndex, const JumpDirection& a_type)
{
	// Logic example:
	// currIndex = (2,2);
	// m_dimCount = 8;
	// a_type = JUMP_TR = MOVE_TR + MOVE_TR = (m_dimCount + 1) + (m_dimCount + 1);
	// => ((row * m_dimCount) + col) + a_type = 2Dto1Dindex
	// => 2Dto1Dindex = ((2 * 8) + 2) + ((8+1)+(8+1)) = (18 + 18) = 36;
	// => targetRow = 2Dto1Dindex % m_dimCount = 36%8 = 4
	// => targetCol = 2Dto1Dindex / m_dimCount = 36/8 = 5
	// #CORRECT ^_^
	int32_t indexIn1D = ((a_cIndex.y * m_dimCount) + a_cIndex.x) + a_type;
	return i32vec2(floor(indexIn1D % m_dimCount), floor(indexIn1D / m_dimCount));
}

const bool LogicHandle::IsWithinBoard(const i32vec2& a_cIndex)
{
	//Check if index exceeds limit
	if (a_cIndex.x >= 0 && a_cIndex.x < m_dimCount &&
		a_cIndex.y >= 0 && a_cIndex.y < m_dimCount)
		return true;

	return false;
}

const bool LogicHandle::IsValidAtoB(const i32vec2& a_cIndex, const i32vec2& a_tIndex)
{
	if (!IsWithinBoard(a_cIndex) || !IsWithinBoard(a_tIndex))
		return false;

	if (!IsInPossibleMoveList(a_tIndex))
		return false;

	return true;
}

const bool LogicHandle::IsValidMove(const i32vec2& a_cIndex, const MoveDirection& a_type)
{
	if (a_type == MoveDirection::MOVE_DIR_SIZE)
		return false;

	i32vec2 tIndex = MoveDirTo2DIndex(a_cIndex, a_type);

	if (!IsWithinBoard(a_cIndex) || !IsWithinBoard(tIndex))
		return false;


	if (GetIDAt(tIndex) != TileID::BLANK_RED)
		return false;

	return true;
}

const bool LogicHandle::IsValidJump(const i32vec2& a_cIndex, const JumpDirection& a_type)
{
	if (a_type == JumpDirection::JUMP_DIR_SIZE)
		return false;

	// Target Index
	i32vec2 tIndex = JumpDirTo2DIndex(a_cIndex, a_type);
	i32vec2 iIndex;

	if (!IsWithinBoard(a_cIndex) || !IsWithinBoard(tIndex))
		return false;

	// If i can move there, it must be vacant, 
	// and thus I can't jump it.
	switch (a_type)
	{
	case LogicHandle::JUMP_TL:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
			return false;
		else
			iIndex = MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TL);
		break;
	case LogicHandle::JUMP_TR:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
			return false;
		else
			iIndex = MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TR);
		break;
	case LogicHandle::JUMP_BR:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
			return false;
		else
			iIndex = MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BR);
		break;
	case LogicHandle::JUMP_BL:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
			return false;
		else
			iIndex = MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BL);
		break;
	default:
		break;
	}

	TileID cID = GetIDAt(a_cIndex);
	TileID iID = GetIDAt(iIndex);

	if (GetIDAt(tIndex) != TileID::BLANK_RED)
		return false;

	if (cID == TileID::PLAYER_RED)
	{
		if (iID == TileID::PLAYER_BLACK || iID == TileID::PLAYER_BLACK_KING)
			return true;
	}
	else if (cID == TileID::PLAYER_RED_KING)
	{
		if (iID == TileID::PLAYER_BLACK || iID == TileID::PLAYER_BLACK_KING)
			return true;
	}
	else if (cID == TileID::PLAYER_BLACK)
	{
		if (iID == TileID::PLAYER_RED || iID == TileID::PLAYER_RED_KING)
			return true;
	}
	else if (cID == TileID::PLAYER_BLACK_KING)
	{
		if (iID == TileID::PLAYER_RED || iID == TileID::PLAYER_RED_KING)
			return true;
	}

	return false;
}
