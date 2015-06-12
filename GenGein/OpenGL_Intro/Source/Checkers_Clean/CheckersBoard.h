#pragma once
#include <vector>
#include <glm\glm.hpp>

class CheckersLogic;
class CheckersVisual;
class CheckersInput;
class CheckersAudio;

////
// Author: Jackson Luff
// Name: CheckersBoard
// Type: class (standard)
// Parent: none
// Description:
// * This class is the collection of
// * logical, visual and inputted 
// * completion of a checkers board.
////
class CheckersBoard
{
public:
	~CheckersBoard();

	// Initalises logic, visual and input
	void Initialise();
	// Updates board input
	void Update(const double_t& a_dt, const f32vec3& a_cursorPosWorld, const int32_t a_isClicked);
	// Renders board visual
	void Render(const glm::mat4& a_camProjView);
private:
	CheckersLogic* m_pLogic;
	CheckersVisual* m_pVisual;
	CheckersInput* m_pInput;
	CheckersAudio* m_pAudio;
};