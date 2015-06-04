#include <iostream>	
#include "CheckersLogic.h"

using namespace BoardData;

CheckersLogic::CheckersLogic()
{}

CheckersLogic::~CheckersLogic()
{}

void CheckersLogic::InitialiseBasicBoard()
{
	bool white = true;

	for (uint i = 0; i < m_tileSegs; ++i)
	{
		for (uint j = 0; j < m_tileSegs; ++j)
		{
			if (!white && j < (m_tileSegs / 2) - 1)
				m_tileIDs[i*m_tileSegs + j] = PLAYER_RED;
			else if (!white && j < (m_tileSegs / 2) + 1)
				m_tileIDs[i*m_tileSegs + j] = BLANK;
			else if (!white && j < m_tileSegs)
				m_tileIDs[i*m_tileSegs + j] = PLAYER_BLUE;
			else
				m_tileIDs[i*m_tileSegs + j] = INVALID;
			white = !white;
		}
		white = !white;
	}

	m_defaultTile = INVALID;
}

const TileType& CheckersLogic::GetPieceAt(c_uint& a_row, c_uint& a_col)
{
	if (!IsValid(a_row, a_col))
		return m_defaultTile;
		
	return m_tileIDs[(a_row * m_tileSegs) + a_col];
}

bool CheckersLogic::IsValidMove(c_uint& a_row, c_uint& a_col, const MoveType& a_type)
{
	if (!IsValid(a_row, a_col)) 
		return false;

	TileType& target = m_tileIDs[((a_row * m_tileSegs) + a_col) + a_type];

	if (target != BLANK)
		return false;

	return true;
}

bool CheckersLogic::IsValidMove(c_uint& a_currR, c_uint& a_currC,
	c_uint& a_destR, c_uint& a_destC)
{
	if (!IsValid(a_currR, a_currC)) return false;

	TileType& target = m_tileIDs[((a_destR * m_tileSegs) + a_destC)];

	if (target != BLANK)
		return false;

	return true;
}

bool CheckersLogic::SetPieceAt(c_uint& a_row, c_uint& a_col, const TileType& a_type)
{
	if (!IsValid(a_row, a_col))
		return false;
	m_tileIDs[(a_row * m_tileSegs) + a_col] = a_type;
	return true;
}

bool CheckersLogic::SetPieceAt(c_uint& a_index, const TileType& a_type)
{
	if (!IsValid(a_index))
		return false;
	m_tileIDs[a_index] = a_type;
	return true;
}

bool CheckersLogic::doMove(c_uint& a_row, c_uint& a_col, const MoveType& a_type)
{
	if (!IsValidMove(a_row, a_col, a_type))
		return false;

	uint cIndex = (a_row * m_tileSegs) + a_col;
	TileType& currType = m_tileIDs[cIndex];

	SetPieceAt(cIndex + a_type, currType);
	SetPieceAt(cIndex, BLANK);

	return true;
}

bool CheckersLogic::doMove(c_uint& a_currR, c_uint& a_currC,
	c_uint& a_destR, c_uint& a_destC)
{
	if (!IsValidMove(a_currR, a_currC, a_destR, a_destC))
		return false;

	uint cIndex = (a_currR * m_tileSegs) + a_currC;
	uint tIndex = (a_destR * m_tileSegs) + a_destC;

	SetPieceAt(tIndex, GetPieceAt(a_currC, a_currR));
	SetPieceAt(cIndex, BLANK);

	return true;
}

bool CheckersLogic::doJump(c_uint& a_row, c_uint& a_col, const MoveType& a_type)
{
	if (!doMove(a_row, a_col, a_type))
		return false;
	uint intercepted = ((a_row * m_tileSegs) + a_col) + (a_type / 2);

	if (m_tileIDs[intercepted] != BLANK)
	{
		SetPieceAt(intercepted, BLANK);
		//add one to tally for player
	}

	return true;
}

bool CheckersLogic::doJump(c_uint& a_currR, c_uint& a_currC,
	c_uint& a_destR, c_uint& a_destC)
{
	if (!doMove(a_currR, a_currC, a_destR, a_destC))
		return false;

	//uint intercepted = ((a_currR * m_tileSegs) + a_col) + (a_type / 2);
	//
	//if (m_tileIDs[intercepted] != BLANK)
	//{
	//	SetPieceAt(intercepted, BLANK);
	//	//add one to tally for player
	//}

	return true;
}

const void CheckersLogic::GenPossibleMoves(c_uint& a_row, c_uint& a_col)
{
	for (uint i = 0; i < MOVETYPESIZE - 1; i++)
	{
		if (doMove(a_row, a_col, (MoveType)i))
			m_possibleMoveIndexs.push_back(CalcTarget(a_row, a_col, (MoveType)i));
	}
}

void CheckersLogic::ClearPossibleMoves()
{
	m_possibleMoveIndexs.clear();
}

// Private funcs:

glm::uvec2 CheckersLogic::CalcTarget(c_uint& a_row, c_uint& a_col, const MoveType& a_type)
{
	uint index = ((a_row * m_tileSegs) + a_col) + a_type;

	return glm::uvec2(index / m_tileSegs, index%m_tileSegs);
}

bool CheckersLogic::IsValid(c_uint& a_index)
{
	//Check if index exceeds limit
	if (a_index < 0 ||
		a_index >= m_tileMaxSize)
		return false;

	if (m_tileIDs[a_index] == INVALID)
		return false;

	return true;
}

bool CheckersLogic::IsValid(c_uint& a_row, c_uint& a_col)
{
	//Check if index exceeds limit
	if (a_row < 0 || a_row >= m_tileSegs ||
		a_row < 0 || a_col >= m_tileSegs)
		return false;

	if (m_tileIDs[a_row * m_tileSegs + a_col] == INVALID)
		return false;

	return true;
}