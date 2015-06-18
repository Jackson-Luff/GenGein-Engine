#pragma once
#include <glm\glm.hpp>
#include "Checkers\CheckersBoard.h"

class LogicHandle;
class AudioHandle;

using TURN_SYS = CheckersBoard::TURN_SYS;

////
// Author: Jackson Luff
// Name: InputHandle
// Type: class (standard)
// Parent: none
// Description:
// * This class is the input director for checkers.
// * It ONLY deals with the input handling and
// * dealing with input detection.
////
class InputHandle
{
public:
	// Constructor
	InputHandle();
	// Deconstructor
	~InputHandle();

	// Initialise creates pointer types to Logic and Visual boards
	void Initialise(LogicHandle* a_pLogic, VisualHandle* a_pVisual, AudioHandle* a_pAudio);
	// Update handles input based on selection
	void Update(const double_t a_dt, const glm::f32vec3& a_cursorPosWorld, const int32_t a_isClicked);

	// Browses Pieces relative to positions to find selected piece.
	const bool BrowsingPieces(const glm::f32vec3& a_cursorPosWorld, const int32_t a_isClicked);

	inline const TURN_SYS& GetTurn() const
		{ return m_turn; }

private:
	// Hacky container to hold
	// previous index of closest
	i32vec2 m_prevClosestIndex;

	// Logic Reference (non-&)
	LogicHandle* m_pLogic;
	// Visual Reference (non-&)
	VisualHandle* m_pVisual;
	// Audio Reference (non-&)
	AudioHandle* m_pAudio;

	// if a piece has been selected yet
	bool m_hasBeenSelected;

	TURN_SYS m_turn;
};