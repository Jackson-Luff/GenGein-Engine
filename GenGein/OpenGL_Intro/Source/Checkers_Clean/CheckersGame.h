#pragma once
#include "Engine\Core\BaseApp.h"

class CheckerBoard;

typedef const int c_int;

class CheckersGame : public BaseApp
{
public:
	CheckersGame(c_int a_width, c_int a_height, c_pChar a_title);
	~CheckersGame();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	CheckerBoard* m_checkerBoard;
	
};