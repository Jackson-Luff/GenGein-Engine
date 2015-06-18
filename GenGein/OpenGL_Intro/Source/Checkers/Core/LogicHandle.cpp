#include <iostream>	
#include "LogicHandle.h"

using MoveDirection = LogicHandle::MoveDirection;
using JumpDirection = LogicHandle::JumpDirection;
using TileID = LogicHandle::TileID;

LogicHandle::LogicHandle()
: m_indexListOfPosMoves(std::vector<i32vec2>(0))
{}

const void LogicHandle::InitialiseBasicBoard()
{
	bool white = true;
	for (uint32_t r = 0; r < m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_dimCount; c++)
		{
			if (!white && c < (m_dimCount / 2) - 1)
				m_boardIDs[r*m_dimCount + c] = TileID::PLAYER_RED;
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

const bool LogicHandle::TryAToB(const i32vec2& a_cIndex, const i32vec2& a_tIndex)
{
	// Is move on possible move list (may not need this)
	if (!IsInPossibleMoveList(a_tIndex))
		return false;

	// These are just triggers. Remember we
	// already checked if they're valid and
	// what not in the generation of them. :)
	bool isMove = false;
	bool isJump = false;

	// determines direction of movement:
	// |A|_|B|  C - A = -1, +1 = MOVE_TL | C - A*2 = -2, -2 = JUMP_TL
	// |_|C|_|  C - B = +1, +1 = MOVE_TR | C - B*2 = -2, -2 = JUMP_TR
	// |D|_|E|  C - E = -1, -1 = MOVE_BR | C - E*2 = -2, -2 = JUMP_BR
	//			C - D = +1, -1 = MOVE_BL | C - D*2 = -2, -2 = JUMP_BL
	// =================================
	i32vec2 dir = a_tIndex - a_cIndex;

	if (abs(dir) == i32vec2(1))
	{
		// It's a move!
		isMove = true;
		SetPieceAt(a_tIndex, GetIDAt(a_cIndex));
		SetPieceAt(a_cIndex, TileID::BLANK_RED);
	}
	else if (abs(dir) == i32vec2(2))
	{
		// It's a jump!
		isJump = true;
		i32vec2 iIndex = a_cIndex + dir / 2;

		if (a_tIndex.y == 7 && GetIDAt(a_cIndex) == TileID::PLAYER_RED)
			SetPieceAt(a_tIndex, TileID::PLAYER_RED_KING);//red king
		else if (a_tIndex.y == 0 && GetIDAt(a_cIndex) == TileID::PLAYER_BLACK)
			SetPieceAt(a_tIndex, TileID::PLAYER_BLACK_KING);//black king
		else
			SetPieceAt(a_tIndex, GetIDAt(a_cIndex));

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
		return true;

	return false;
}

const void LogicHandle::GeneratePossibleMoves(const i32vec2& a_cIndex)
{
	if (GetIDAt(a_cIndex) == TileID::BLANK_BLACK || GetIDAt(a_cIndex) == TileID::BLANK_RED)
		return;

	TileID ID = GetIDAt(a_cIndex);

	// Red pieces can't move back, thus move up the board 
	if (ID == TileID::PLAYER_RED)
	{
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TL));
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TR));

		if (IsValidJump(a_cIndex, JumpDirection::JUMP_TL))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TL));
		if (IsValidJump(a_cIndex, JumpDirection::JUMP_TR))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TR));
	}

	// Black pieces can't move forward, thus move down the board 
	if (ID == TileID::PLAYER_BLACK)
	{
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BR));
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BL));

		if (IsValidJump(a_cIndex, JumpDirection::JUMP_BR))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BR));
		if (IsValidJump(a_cIndex, JumpDirection::JUMP_BL))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BL));
	}

	// King has all authorisation, and thus every move is possible.
	if (ID == TileID::PLAYER_RED_KING || ID == TileID::PLAYER_BLACK_KING)
	{
		//Moves
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TL));
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_TR));

		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BR));
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
			m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(a_cIndex, MoveDirection::MOVE_BL));

		//Jumps
		if (IsValidJump(a_cIndex, JumpDirection::JUMP_TL))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TL));
		if (IsValidJump(a_cIndex, JumpDirection::JUMP_TR))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_TR));

		if (IsValidJump(a_cIndex, JumpDirection::JUMP_BR))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BR));
		if (IsValidJump(a_cIndex, JumpDirection::JUMP_BL))
			m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(a_cIndex, JumpDirection::JUMP_BL));
	}

	if (m_indexListOfPosMoves.size() == 0)
		printf("No valid moves at index; x:%i y:%i \n", a_cIndex.x, a_cIndex.y);
}

const std::vector<i32vec2> LogicHandle::GeneratePossibleMoves(const TileID& a_ID)
{
	if (a_ID == TileID::BLANK_RED || a_ID == TileID::BLANK_BLACK)
		return std::vector<i32vec2>(0);

	std::vector<i32vec2> outList;

	for (int32_t r = 0; r < m_dimCount; r++)
	{
		for (int32_t c = 0; c < m_dimCount; c++)
		{
			i32vec2 cInd = i32vec2(r, c);

			if (GetIDAt(cInd) == TileID::BLANK_RED ||
				GetIDAt(cInd) == TileID::BLANK_BLACK)
				continue;

			// Red pieces can't move back, thus move up the board 
			if (a_ID == TileID::PLAYER_RED)
			{
				if (IsValidMove(cInd, MoveDirection::MOVE_TL))
					outList.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TL));
				if (IsValidMove(cInd, MoveDirection::MOVE_TR))
					outList.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TR));

				if (IsValidJump(cInd, JumpDirection::JUMP_TL))
					outList.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TL));
				if (IsValidJump(cInd, JumpDirection::JUMP_TR))
					outList.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TR));
			}

			// Black pieces can't move forward, thus move down the board 
			if (a_ID == TileID::PLAYER_BLACK)
			{
				if (IsValidMove(cInd, MoveDirection::MOVE_BR))
					outList.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BR));
				if (IsValidMove(cInd, MoveDirection::MOVE_BL))
					outList.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BL));

				if (IsValidJump(cInd, JumpDirection::JUMP_BR))
					outList.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BR));
				if (IsValidJump(cInd, JumpDirection::JUMP_BL))
					outList.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BL));
			}

			// King has all authorisation, and thus every move is possible.
			if (a_ID == TileID::PLAYER_RED_KING || a_ID == TileID::PLAYER_BLACK_KING)
			{
				//Moves
				if (IsValidMove(cInd, MoveDirection::MOVE_TL))
					m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TL));
				if (IsValidMove(cInd, MoveDirection::MOVE_TR))
					m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_TR));

				if (IsValidMove(cInd, MoveDirection::MOVE_BR))
					m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BR));
				if (IsValidMove(cInd, MoveDirection::MOVE_BL))
					m_indexListOfPosMoves.push_back(MoveDirTo2DIndex(cInd, MoveDirection::MOVE_BL));

				//Jumps												 
				if (IsValidJump(cInd, JumpDirection::JUMP_TL))
					m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TL));
				if (IsValidJump(cInd, JumpDirection::JUMP_TR))
					m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_TR));

				if (IsValidJump(cInd, JumpDirection::JUMP_BR))
					m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BR));
				if (IsValidJump(cInd, JumpDirection::JUMP_BL))
					m_indexListOfPosMoves.push_back(JumpDirTo2DIndex(cInd, JumpDirection::JUMP_BL));
			}
		}
	}

	if (outList.size() == 0)
		printf("No valid moves for ID; %i \n", a_ID);

	return outList;
}

const bool LogicHandle::IsInPossibleMoveList(const i32vec2& a_tIndex)
{
	if (std::find(m_indexListOfPosMoves.begin(),
		m_indexListOfPosMoves.end(), a_tIndex) != m_indexListOfPosMoves.end())
		return true;

	return false;
}

const void LogicHandle::ClearPossibleMovesList()
{
	m_indexListOfPosMoves.clear();
}

// Private funcs:

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
	if (a_cIndex.x > 0 || a_cIndex.x < m_dimCount ||
		a_cIndex.y > 0 || a_cIndex.y < m_dimCount)
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
