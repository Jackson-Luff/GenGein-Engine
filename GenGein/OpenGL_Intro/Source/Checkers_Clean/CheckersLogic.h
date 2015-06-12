#pragma once
#include <vector>
#include <glm\glm.hpp>

using namespace glm;

////
// Author: Jackson Luff
// Name: CheckersLogic
// Type: class (standard)
// Parent: none
// Description:
// * This class is the backbone of a checkers game.
// * It ONLY deals with the logical states and 
// * is specifically crafted for checkers logic/ruling.
////
class CheckersLogic
{
public:
	// Non-changable important data
	const float32_t m_tileSize = 0.5f;
	static const int32_t m_dimCount = 8;
	static const int32_t m_tileMaxSize = m_dimCount * m_dimCount;

	// Index offsets for move logic:
	// |_|_|_|_| # C(1,2) - Current Location
	// |_|_|_|_| # T(3,2) - Target Location
 	// |_|_|T|_| # => T = C + (dimensions + 1);
	// |_|C|_|_| # => T = C + MOVE_TR;
	enum MoveDirection : int32_t
	{
		MOVE_TL =  m_dimCount - 1,
		MOVE_TR =  m_dimCount + 1,
		MOVE_BR = -m_dimCount + 1,
		MOVE_BL = -m_dimCount - 1,
		MOVE_DIR_SIZE = 4,
	};

	// Index offset for jump logic:
	// |_|_|_|_| # C(1,4) - Current Location  
	// |_|T|_|_| # T(3,2) - Target Location
	// |_|_|_|_| # => T = C + ((m_dimCount - 1) + (m_dimCount - 1));
	// |_|_|_|C| # => T = C + JUMP_TL;
	enum JumpDirection : int32_t
	{
		JUMP_TL = MOVE_TL + MOVE_TL,
		JUMP_TR = MOVE_TR + MOVE_TR,
		JUMP_BR = MOVE_BR + MOVE_BR,
		JUMP_BL = MOVE_BL + MOVE_BL,
		JUMP_DIR_SIZE = 4,
	};

	// Maintains board based on ID system (8x8):
	// { -1, +1, -1, +1, -1, +1, -1, +1 }
	// { +1, -1, +1, -1, +1, -1, +1, -1 }
	// { -1, +1, -1, +1, -1, +1, -1, +0 }
	// { +0, -1, +0, -1, +0, -1, +0, -1 }
	// { -1, +0, -1, +0, -1, +0, -1, +0 }
	// { +2, -1, +2, -1, +2, -1, +2, -1 }
	// { -1, +2, -1, +2, -1, +2, -1, +2 }
	// { +2, -1, +2, -1, +2, -1, +2, -1 }
	enum class TileID : char
	{
		BLACK = -1,
		WHITE,
		PLAYER_RED,
		PLAYER_BLACK,
	};

	// Constructor
	CheckersLogic();

	// Initialises a checker board 
	// with assigned player pieces.
	const void InitialiseBasicBoard();

	// Grabs via ref of ID at index specified 
	const TileID& GetIDAt(const i32vec2& a_cIndex);

	// Returns 2D coords of target location via 2D coords of current via move type 
	const i32vec2 GetTargetIndex(const i32vec2& a_cIndex, const MoveDirection& a_type);
	// Returns 2D coords of target location via 2D coords of current via jump type 
	const i32vec2 GetTargetIndex(const i32vec2& a_cIndex, const JumpDirection& a_type);

	// Generic setter for piece based on index and what you wish you set it to.
	const bool SetPieceAt(const i32vec2& a_cIndex, const TileID& a_type);

	// Attempts a more A to B approach to movement
	const bool TryAToB(const i32vec2& a_cIndex, const i32vec2& a_tIndex);
	// Move to a specific location based on requested input
	const bool TryMove(const i32vec2& a_cIndex, const MoveDirection& a_type);
	// Jump to a specific location based on requested input
	const bool TryJump(const i32vec2& a_cIndex, const JumpDirection& a_type);

	// Generates list of possible moves at indexed location
	// NOTE: List is stored in m_listOfValidMoves
	const void GeneratePossibleMoves(const i32vec2& a_cIndex);
	// Clears Possible Moves
	const void ClearPossibleMovesList();
	// Checks if the target location is within possible move list
	const bool isInPossibleMoveList(const i32vec2& a_tIndex);
	// Gets the list of indexs for valid positions
	inline const std::vector<i32vec2>& GetPossibleMovesList() const 
		{ return m_listOfValidMoves; }
	
private:
	
	// Checks if index(s) given are within range of dimension limitation
	const bool isWithinBoard(const i32vec2& a_cIndex);

	// Checks to see if there is a valid move/jump from point A to B
	const bool IsValidAtoB(const i32vec2& a_cIndex, const i32vec2& a_tIndex);
	// Checks to see if there is a valid move based on move type
	const bool IsValidMove(const i32vec2& a_cIndex, const MoveDirection& a_type);
	// Checks to see if there is a valid jump based on jump type
	const bool IsValidJump(const i32vec2& a_cIndex, const JumpDirection& a_type);

	// Representation of board via ID system
	TileID m_boardIDs[m_tileMaxSize];
	// Is always invalid (used in checks for; get via ref)
	const TileID m_nullTile = TileID::BLACK;
	// Holds index information of valid move(s)
	std::vector<glm::i32vec2> m_listOfValidMoves;
};