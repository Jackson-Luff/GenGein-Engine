#include <iostream>	
#include "Board.h"

using namespace BoardData;


Board::Board()
{
}


Board::~Board()
{
}

void Board::Initialise()
{
	for (uint r = 0; r < rowCount; r++)
	{
		for (uint c = 0; c < colCount; c++)
		{
			//m_tiles[r][c] = TileData();

			glm::vec3 pos = vec3(r * tileSize, 1, c * tileSize);
			//pos.z -= halfSize;
			//pos.z += tileSize / 2.0f;
			//pos.x -= tileSize * 3 + tileSize / 2.0f;

			if ((r+c) % 2 == 0)
				m_tiles[r][c].type = BLANK;
			else
				m_tiles[r][c].type = INVALID;

			m_tiles[r][c].position = pos;

			printf(" %d ", m_tiles[r][c].type);
		}
		printf("\n");
	}
	printf("\n");
}

bool Board::AddPieceAtIndex(const TileType& a_type,
	c_uint& a_row, c_uint& a_col)
{
	if (!IsValid(a_row, a_col))	return false;
		
	m_tiles[a_row][a_col].type = a_type;
	return true;
}

TileData& Board::GetPieceAtIndex(c_uint& a_row, c_uint& a_col)
{
	if (IsValid(a_row, a_col)) 
		return m_tiles[a_row][a_col];

	return TileData();
}

bool Board::RemovePieceAtIndex(c_uint& a_row, c_uint& a_col)
{
	return true;
}

bool Board::RemovePieceAtPosition(const vec3 a_pos)
{
	return true;
}

bool Board::IsValidMove()
{
	return true;
}

bool Board::IsValidJump()
{
	return true;
}

bool Board::IsValid(c_uint& a_row, c_uint& a_col)
{
	if (a_row > rowCount - 1 || a_col > colCount - 1)
		return false;

	//if (m_tiles[a_row][a_col].type == INVALID)
	//	return false;

	return true;
}

bool Board::IsValid(const vec3 a_pos)
{
	for (uint r = 0; r < rowCount; r++)
	{
		for (uint c = 0; c < colCount; c++)
		{
			if (m_tiles[r][c].position == a_pos)
				return true;
		}
	}

	return false;
}

bool Board::IsValid(const TileType& a_type)
{
	if (a_type != INVALID) return true;
	return false;
}
