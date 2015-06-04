#pragma once
#include <vector>
#include <glm\glm.hpp>

typedef unsigned int uint;
typedef const unsigned int c_uint;

using glm::vec3;


namespace BoardData
{
	enum MoveType
	{
		STRIDE_TL = 7,
		STRIDE_TR = 9,
		STRIDE_BR = -7,
		STRIDE_BL = -9,
		JUMP_TL = STRIDE_TL + STRIDE_TL,
		JUMP_TR = STRIDE_TR + STRIDE_TR,
		JUMP_BR = STRIDE_BR + STRIDE_BR,
		JUMP_BL = STRIDE_BL + STRIDE_BL,
		MOVETYPESIZE
	};
	enum TileType
	{
		// Invalid Types
		INVALID = -1,

		BLANK = 0,
		AI = 1,
		PLAYER_RED = 2,
		PLAYER_BLUE = PLAYER_RED + 1,
	};
}

using namespace BoardData;

class CheckersLogic
{
public:
	CheckersLogic();
	~CheckersLogic();

	virtual void InitialiseBasicBoard();

	const TileType& GetPieceAt(c_uint& a_row, c_uint& a_col);

	virtual bool SetPieceAt(c_uint& a_row, c_uint& a_col, const TileType& a_type);
	virtual bool SetPieceAt(c_uint& a_index, const TileType& a_type);

	virtual bool doMove(c_uint& a_currR, c_uint& a_currC, c_uint& a_destR, c_uint& a_destC);
	virtual bool doMove(c_uint& a_row, c_uint& a_col, const MoveType& a_type);
	virtual bool doJump(c_uint& a_currR, c_uint& a_currC, c_uint& a_destR, c_uint& a_destC);
	virtual bool doJump(c_uint& a_row, c_uint& a_col, const MoveType& a_type);

	inline const uint& GetRowCount() { return m_tileSegs; }
	inline const uint& GetColCount() { return m_tileSegs; }
	inline const float& GetTileSize() { return m_tileSize; }
	inline const uint& GetBoardSize() { return m_tileMaxSize; }
	inline const std::vector<glm::uvec2>& GetPossibleMoves() { return m_possibleMoveIndexs; }

	// Checks if the target location is within possible move list
	const bool isPossibleMove(c_uint& a_targR, c_uint& a_targC);
	// Generates list of possible moves at indexed location
	const void GenPossibleMoves(c_uint& a_row, c_uint& a_col);
	// Clears Possible Moves
	void ClearPossibleMoves();
protected:
	const float m_tileSize = 0.5f;
	static const uint m_tileSegs = 8;
	static const uint m_tileMaxSize = m_tileSegs * m_tileSegs;
	TileType m_tileIDs[m_tileMaxSize];
	TileType m_defaultTile;
	std::vector<glm::uvec2> m_possibleMoveIndexs;
private:
	glm::uvec2 CalcTarget(c_uint& a_row, c_uint& a_col, const MoveType& a_type);

	bool IsValid(c_uint& a_index);
	bool IsValid(c_uint& a_row, c_uint& a_col);
	bool IsValidMove(c_uint& a_row, c_uint& a_col, const MoveType& a_type);
	bool IsValidMove(c_uint& a_currR, c_uint& a_currC, c_uint& a_destR, c_uint& a_destC);
};