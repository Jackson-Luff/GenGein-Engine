#include <iostream>	
#include "CheckersLogic.h"

using MoveDirection = CheckersLogic::MoveDirection;
using JumpDirection = CheckersLogic::JumpDirection;
using TileID		= CheckersLogic::TileID;

CheckersLogic::CheckersLogic()
	: m_listOfValidMoves(std::vector<i32vec2>(0))
{}

const void CheckersLogic::InitialiseBasicBoard()
{
	bool white = true;
	for (uint32_t r = 0; r < m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_dimCount; c++)
		{
			if (!white && c < (m_dimCount / 2) - 1)
				m_boardIDs[r*m_dimCount + c] = TileID::PLAYER_RED;
			else if (!white && c < (m_dimCount / 2) + 1)
				m_boardIDs[r*m_dimCount + c] = TileID::WHITE;
			else if (!white && c < m_dimCount)
				m_boardIDs[r*m_dimCount + c] = TileID::PLAYER_BLACK;
			else
				m_boardIDs[r*m_dimCount + c] = TileID::BLACK;
			white = !white;
		}
		white = !white;
	}
}

const TileID& CheckersLogic::GetIDAt(const i32vec2& a_cIndex)
{
	if (!isWithinBoard(a_cIndex))
		return m_nullTile;
	
	return m_boardIDs[(a_cIndex.x * m_dimCount) + a_cIndex.y];
}

const i32vec2 CheckersLogic::GetTargetIndex(const i32vec2& a_cIndex, const MoveDirection& a_type)
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

const i32vec2 CheckersLogic::GetTargetIndex(const i32vec2& a_cIndex, const JumpDirection& a_type)
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

const bool CheckersLogic::SetPieceAt(const i32vec2& a_cIndex, const TileID& a_type)
{
	if (!isWithinBoard(a_cIndex))
		return false;

	if (GetIDAt(a_cIndex) == TileID::BLACK)
	{
		printf("Cannot Set Piece of Black (invalid type).");
		return false;
	}

	m_boardIDs[(a_cIndex.x * m_dimCount) + a_cIndex.y] = a_type;
	return true;
}

const bool CheckersLogic::TryAToB(const i32vec2& a_cIndex, const i32vec2& a_tIndex)
{ 
	// Is move on possible move list (may not need this)
	if (!isInPossibleMoveList(a_tIndex))
		return false;

	SetPieceAt(a_tIndex, GetIDAt(a_cIndex));
	SetPieceAt(a_cIndex, TileID::WHITE);
	return true;
}

const bool CheckersLogic::TryMove(const i32vec2& a_cIndex, const MoveDirection& a_type)
{
	i32vec2 tIndex = GetTargetIndex(a_cIndex, a_type);

	// Is move on possible move list (may not need this)
	if (!isInPossibleMoveList(tIndex))
		return false;

	SetPieceAt(tIndex, GetIDAt(a_cIndex));
	SetPieceAt(a_cIndex, TileID::WHITE);
	return true;
}

const bool CheckersLogic::TryJump(const i32vec2& a_cIndex, const JumpDirection& a_type)
{
	// Target index
	i32vec2 tIndex = GetTargetIndex(a_cIndex, a_type);
	
	if (!IsValidJump(a_cIndex, a_type))
		return false;

	//Collect intercepted piece
	SetPieceAt(tIndex, GetIDAt(a_cIndex));
	//SetPieceAt(iIndex,	 TileID::WHITE);
	SetPieceAt(a_cIndex, TileID::WHITE);

	return true;
}

const void CheckersLogic::GeneratePossibleMoves(const i32vec2& a_cIndex)
{
	if (GetIDAt(a_cIndex) == TileID::BLACK ||
		GetIDAt(a_cIndex) == TileID::WHITE)
		return;

	// Move Tests
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, MoveDirection::MOVE_TL));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, MoveDirection::MOVE_TR));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, MoveDirection::MOVE_BR));
	if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, MoveDirection::MOVE_BL));
	// Jump Tests 
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_TL))
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, JumpDirection::JUMP_TL));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_TR))	 				 
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, JumpDirection::JUMP_TR));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_BR))	 				 
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, JumpDirection::JUMP_BR));
	if (IsValidJump(a_cIndex, JumpDirection::JUMP_BL))	  				 
		m_listOfValidMoves.push_back(GetTargetIndex(a_cIndex, JumpDirection::JUMP_BL));

}

const bool CheckersLogic::isInPossibleMoveList(const i32vec2& a_tIndex)
{
	if (std::find(m_listOfValidMoves.begin(), m_listOfValidMoves.end(), a_tIndex) != m_listOfValidMoves.end())
		return true;

	return false;
}

const void CheckersLogic::ClearPossibleMovesList()
{
	m_listOfValidMoves.clear();
}

// Private funcs:

const bool CheckersLogic::isWithinBoard(const i32vec2& a_cIndex)
{
	//Check if index exceeds limit
	if (a_cIndex.x >= 0 || a_cIndex.x < m_dimCount ||
		a_cIndex.y >= 0 || a_cIndex.y < m_dimCount)
		return true;

	return false;
}

const bool CheckersLogic::IsValidAtoB(const i32vec2& a_cIndex, const i32vec2& a_tIndex)
{
	if (!isWithinBoard(a_cIndex) || !isWithinBoard(a_tIndex))
		return false;

	if (!isInPossibleMoveList(a_tIndex))
		return false;

	return true;
}

const bool CheckersLogic::IsValidMove(const i32vec2& a_cIndex, const MoveDirection& a_type)
{
	if (a_type == MoveDirection::MOVE_DIR_SIZE)
		return false;

	i32vec2 tIndex = GetTargetIndex(a_cIndex, a_type);

	if (!isWithinBoard(a_cIndex) || !isWithinBoard(tIndex))
		return false;

	if (GetIDAt(tIndex) != TileID::WHITE)
		return false;

	return true;
}

const bool CheckersLogic::IsValidJump(const i32vec2& a_cIndex, const JumpDirection& a_type)
{
	if (a_type == JumpDirection::JUMP_DIR_SIZE)
		return false;

	// Target Index
	i32vec2 tIndex = GetTargetIndex(a_cIndex, a_type);
	i32vec2 iIndex;

	if (!isWithinBoard(a_cIndex) || !isWithinBoard(tIndex))
		return false;

	switch (a_type)
	{
	case CheckersLogic::JUMP_TL:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TL))
			return false;
		else
			iIndex = GetTargetIndex(a_cIndex, MoveDirection::MOVE_TL);
		break;
	case CheckersLogic::JUMP_TR:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_TR))
			return false;
		else
			iIndex = GetTargetIndex(a_cIndex, MoveDirection::MOVE_TR);
		break;
	case CheckersLogic::JUMP_BR:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BR))
			return false;
		else
			iIndex = GetTargetIndex(a_cIndex, MoveDirection::MOVE_BR);
		break;
	case CheckersLogic::JUMP_BL:
		if (IsValidMove(a_cIndex, MoveDirection::MOVE_BL))
			return false;
		else
			iIndex = GetTargetIndex(a_cIndex, MoveDirection::MOVE_BL);
		break;
	default:
		break;
	}

	if (GetIDAt(iIndex) == TileID::WHITE)
		return false;

	if (GetIDAt(a_cIndex) == TileID::PLAYER_RED)
	{
		if (GetIDAt(iIndex) != TileID::PLAYER_BLACK)
			return false;
	}
	else if (GetIDAt(a_cIndex) == TileID::PLAYER_BLACK)
	{
		if (GetIDAt(iIndex) != TileID::PLAYER_RED)
			return false;
	}

	if (GetIDAt(tIndex) != TileID::WHITE)
		return false;

	return true;
}