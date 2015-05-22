#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;

using glm::vec3;
using glm::vec2;

namespace CHECKERS_DATA
{
	enum PIECE_TYPE : char
	{
		// Possible player states
		PLAYER_RED = 1,
		PLAYER_BLUE = 2,
		PLAYER_KING = 3,
		AI_COMPONENT = 4,

		// Blanks
		BLANKWHITE = 5,
		
		// Invalid moves
		BLANKBLACK = -1,
		UNKNOWN = -1
	};

	enum MOVE_TYPE : char
	{
		INVALID = -1,
		STRIDE = 0,
		JUMP = 1,
	};

	struct BoardTile
	{
		uint tileID;
		glm::vec3 position;
	};


	struct BoardData
	{
		vec3 position;
		vec2 uv;
	};

	struct SelectedPiece
	{
		int index;
		bool isSelected;
		vec3 homePosition;
	};
	
	struct PieceData
	{
		glm::vec3 position;
		glm::vec3 colour;
		PIECE_TYPE type;
	};
}

class CheckerData
{
public:
	CheckerData();
	//CheckerData();
	~CheckerData();

	enum DIRECTION : char
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOT_LEFT,
		BOT_RIGHT
	};

	enum TileID : char
	{
		// Invalid types
		BLANK = -1,
		UNKNOWN = -1,
		
		// Known types
		PLAYER = 0, 
		AI = 1,

	};

	struct CheckerTile
	{
		TileID id;
	};

	uint WorldPosToDataPos(const vec3& a_pos)
	{
		//for (int )
		return 0;
	}

	vec3 DataPosToWorldPos(const uint& a_xIndex, const uint& a_yIndex)
	{
		return vec3(0);
	}

	bool isValidPiece(const uint& a_tileIndex)
	{
		if (m_tiles[a_tileIndex].id == -1) return false;
		return true;
	}

	bool isValidMove()
	{

	}

	bool isValidJump()
	{

	}

	bool isInBounds(const vec3 a_pos)
	{

	}

	bool isOccupied(const vec3& a_pos)
	{
		if (m_tiles[WorldPosToDataPos(a_pos)].id == BLANK)
			return false;
		return true;
	}

	void doMove(DIRECTION a_dir, const uint& a_tileIndex)
	{
		if (!isValidPiece(a_tileIndex)) return;

		uint colPlusOne = colCount + 1;
		uint colMinusOne = colCount - 1;

		switch (a_dir)
		{
		case CheckerData::TOP_LEFT:
			if (isValidMove())
				m_tiles[a_tileIndex + colMinusOne];
			break;
		case CheckerData::TOP_RIGHT:
			m_tiles[a_tileIndex + colPlusOne];
			break;
		case CheckerData::BOT_LEFT:
			m_tiles[a_tileIndex - colMinusOne];
			break;
		case CheckerData::BOT_RIGHT:
			m_tiles[a_tileIndex - colPlusOne];
			break;
		default:
			break;
		}
	}

private:
	const uint colCount = 8;
	const uint rowCount = 8;
	CheckerTile m_tiles[64];
};