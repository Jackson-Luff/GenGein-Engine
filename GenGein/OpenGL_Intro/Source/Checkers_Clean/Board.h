#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;
typedef const unsigned int c_uint;

using glm::vec3;


namespace BoardData
{
	enum TileType
	{
		// Invalid Types
		INVALID = -1,

		BLANK = 0,
		AI = 1,
		PLAYER_RED = 2,
		PLAYER_BLUE = PLAYER_RED + 1,
	};

	struct TileData
	{
		TileType type;
		vec3 position;
	};
}

using namespace BoardData;

class Board
{
public:
	Board();
	~Board();

	virtual void Initialise();
	
	virtual bool IsValidMove();
	virtual bool IsValidJump();

	bool AddPieceAtIndex(const TileType& a_type, c_uint& a_row, c_uint& a_col);
	TileData& GetPieceAtIndex(c_uint& a_row, c_uint& a_col);

	bool RemovePieceAtIndex(c_uint& a_row, c_uint& a_col);
	bool RemovePieceAtPosition(const vec3 a_pos);
	bool RemoveAllOfType(const TileType& a_type);

	inline uint GetRowCount() { return rowCount; }
	inline uint GetColCount() { return colCount; }

private:
	bool IsValid(c_uint& a_row, c_uint& a_col);
	bool IsValid(const vec3 a_pos);
	bool IsValid(const TileType& a_type);

	const uint tileSize = 1;
	const uint rowCount = 8;
	const uint colCount = 8;
	TileData m_tiles[8][8];
};

