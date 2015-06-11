#pragma once
#include "Engine\Core\BaseApp.h"

class CheckersBoard;

////
// Author: Jackson Luff
// Name: GameOfCheckers
// Type: class (standard)
// Parent: none
// Description:
// * This Application simply
// * runs a really pretty version
// * of 3D checkers.
////
class GameOfCheckers : public BaseApp
{
public:
	GameOfCheckers(const int32_t a_width, const int32_t a_height, const char* a_title);
	~GameOfCheckers();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double_t& a_dt);
	// Render things to screen
	virtual void Render();
private:
	CheckersBoard* m_checkerBoard;
};