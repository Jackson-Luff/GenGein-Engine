//#pragma once
//#include <glm\glm.hpp>
//
//typedef unsigned int uint;
//
//using glm::vec3;
//using glm::vec2;
//
//namespace CHECKERS_DATA
//{
//	enum PIECE_TYPE : char
//	{
//		// Possible player states
//		PLAYER_RED = 1,
//		PLAYER_BLUE = 2,
//		PLAYER_KING = 3,
//		AI_COMPONENT = 4,
//
//		// Blanks
//		BLANKWHITE = 5,
//
//		// Invalid moves
//		BLANKBLACK = -1,
//		UNKNOWN = -1
//	};
//
//	struct BoardTile
//	{
//		uint type;
//		glm::vec3 position;
//	};
//
//	struct BoardData
//	{
//		vec3 position;
//		vec2 uv;
//	};
//
//	struct SelectedPiece
//	{
//		int index;
//		bool isSelected;
//		vec3 homePosition;
//	};
//
//	struct PieceData
//	{
//		glm::vec3 position;
//		glm::vec3 colour;
//		PIECE_TYPE type;
//	};
//}
//
//namespace BoardData
//{
//	enum MoveType
//	{
//		STRIDE_TL = 7,
//		STRIDE_TR = 9,
//		STRIDE_BR = -7,
//		STRIDE_BL = -9,
//		JUMP_TL = STRIDE_TL + STRIDE_TL,
//		JUMP_TR = STRIDE_TR + STRIDE_TR,
//		JUMP_BR = STRIDE_BR + STRIDE_BR,
//		JUMP_BL = STRIDE_BL + STRIDE_BL,
//	};
//
//	enum TileType
//	{
//		// Invalid Types
//		INVALID = -1,
//
//		BLANK = 0,
//		AI = 1,
//		PLAYER_RED = 2,
//		PLAYER_BLUE = PLAYER_RED + 1,
//	};
//
//	struct TileData
//	{
//		TileType type;
//		vec3 position;
//	};
//
//}