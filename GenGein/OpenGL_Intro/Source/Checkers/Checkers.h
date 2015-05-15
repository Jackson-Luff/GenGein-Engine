#pragma once
#include "Engine\Core\BaseApp.h"
#include "CheckersData.h"

class Player;

using glm::vec3;
using glm::vec2;

class Checkers : public BaseApp
{
public:
	Checkers();
	Checkers(c_int a_width, c_int a_height, c_pChar a_title);
	~Checkers();

	void GenCheckerBoardData(const float a_boardSize, const float a_tileCount);

	// Initialise things
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
		
	CHECKERS_DATA::BoardData verts[4];

	uint* m_checkerProgID;
	uint m_VAO;

	float m_boardSize, m_tileCount, m_tileSize;
	std::vector<glm::vec3> m_possiblePositions;

	Player* m_playerOne;
	Player* m_playerTwo; 
};