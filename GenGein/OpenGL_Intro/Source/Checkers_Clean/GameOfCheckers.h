#pragma once
#include "Engine\Core\BaseApp.h"

class CheckersBoard;

typedef const int c_int;

class GameOfCheckers : public BaseApp
{
public:
	GameOfCheckers(c_int a_width, c_int a_height, c_pChar a_title);
	~GameOfCheckers();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	CheckersBoard* m_checkerBoard;
};