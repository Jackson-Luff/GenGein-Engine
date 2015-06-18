#pragma once
#include <vector>
#include <glm\glm.hpp>

class LogicHandle;
class VisualHandle;
class InputHandle;
class AudioHandle;
class MCTS;

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
	enum class TURN_SYS : uint32_t
	{
		PLAYER,
		AI
	};

	// Deconstructor
	~CheckersBoard();

	// Initalises logic, visual and input
	void Initialise();
	// Updates board input
	void Update(const double_t& a_dt, const glm::f32vec3& a_cursorPosWorld, const int32_t a_isClicked, const int32_t a_isRKeyDown);
	// Renders board visual
	void Render(const glm::mat4& a_camProjView);
	// 
	void Shutdown();

private:
	
	LogicHandle* m_pLogic;
	VisualHandle* m_pVisual;
	InputHandle* m_pInput;
	AudioHandle* m_pAudio;
	MCTS* m_pAI;

	TURN_SYS m_turn;
};