#pragma once
#include <glm\glm.hpp>

class CheckersLogic;

////
// Author: Jackson Luff
// Name: CheckersInput
// Type: class (standard)
// Parent: none
// Description:
// * This class is the input director for checkers.
// * It ONLY deals with the input handling and
// * dealing with input detection.
////
class CheckersInput
{
public:
	// Constructor
	CheckersInput();
	// Deconstructor
	~CheckersInput();

	// Initialise creates pointer types to Logic and Visual boards
	void Initialise(CheckersLogic* a_pLogicBoard, CheckersVisual* a_pVisualBoard);
	// Update handles input based on selection
	void Update(const double_t a_dt, const glm::f32vec3& a_cursorPosWorld, const int32_t a_isClicked);

	// Browses Pieces relative to positions to find selected piece.
	const bool BrowsingPieces(const glm::f32vec3& a_cursorPosWorld, const int32_t a_isClicked);

private:
	// Logic Reference (non-&)
	CheckersLogic* m_pLogic;
	// Visual Reference (non-&)
	CheckersVisual* m_pVisual;
	// if a piece has been selected yet
	bool m_hasBeenSelected;
};