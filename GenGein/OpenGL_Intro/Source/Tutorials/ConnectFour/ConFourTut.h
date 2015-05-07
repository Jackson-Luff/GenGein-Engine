#pragma once
#include "Engine\Core\GLApplication.h"


class ConFourTut : public GLApplication
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
	virtual void Update(const double a_dt);
	// Render to GUI
	virtual void Render();

private:
	Game* m_game;
	AI* m_ai;
};

#include <vector>

// base class to define a zero-sum game with two opponents
// actions are defined as int's
class Game {
public:
	enum State {
		UNKNOWN,
		PLAYER_ONE,
		PLAYER_TWO,
		DRAW,
	};
	Game() :
		m_currentPlayer(PLAYER_ONE),
		m_currentOpponent(PLAYER_TWO) {
	}
	virtual ~Game() {}
	// returns wether someone is winning or not
	virtual State getCurrentGameState() const = 0;
	// these switch whenever an action is made
	State getCurrentPlayer() const { return m_currentPlayer; }
	State getCurrentOpponent() const { return m_currentOpponent; }
	// get a list of valid actions for current player
	virtual void getValidActions(std::vector<int>& actions) const = 0;
	virtual bool isActionValid(int action) const = 0;
	// performs an action for the current player
	// and switches current player
	virtual void performAction(int action) = 0;
	// draw the game
	virtual void draw() const = 0;
	// clones the current game state
	virtual Game* clone() const = 0;
protected:
	State m_currentPlayer;
	State m_currentOpponent;
};

// base class for an A.I. opponent
class AI {
public:
	AI() {}
	virtual ~AI() {}
	// makes a decision for the current game
	virtual int makeDecision(const Game& game) = 0;
};

// an A.I. opponent that chooses purely random actions to perform
class RandomAI : public AI {
public:
	RandomAI() {}
	virtual ~RandomAI() {}
	// randomly choose an action to perform
	virtual int makeDecision(const Game& game) {
		std::vector<int> actions;
		game.getValidActions(actions);
		if (actions.size() > 0)
			return actions[rand() % actions.size()];
		else
			return -1;
	}
};

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