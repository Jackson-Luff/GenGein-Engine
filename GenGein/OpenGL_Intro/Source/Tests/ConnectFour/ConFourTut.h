#pragma once
#include <vector>
#include "Tests\ConnectFour\AIBase.h"
#include "Engine\Core\BaseApp.h"

using namespace glm;

class MCTS : public AI {
public:
	MCTS(int playouts) :
		m_playouts(playouts) {
	}
	virtual ~MCTS() {}
	virtual int makeDecision(const Game& game);
private:
	// the number of times it will simulate a game
	int m_playouts;
};

class ConFourTut : public BaseApp
{
public:
	ConFourTut();
	ConFourTut(c_int a_width, c_int a_height, c_pChar a_title);
	~ConFourTut();

	// Initialised content 
	virtual void StartUp();
	// Deconstructed content
	virtual void ShutDown();
	// Update loop
	virtual void Update(const double_t a_dt);
	// Render to GUI
	virtual void Render();

private:
	Game* m_game;
	AI* m_ai;
	glm::vec3 m_pickPosition;
};

