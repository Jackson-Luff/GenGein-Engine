#pragma once
#include <vector>
#include <glm\glm.hpp>

class CheckersLogic;
class CheckersVisual;
class CheckersInput;

typedef const double c_dub;

class CheckersBoard
{
public:
	CheckersBoard();
	~CheckersBoard();

	void Initialise();
	void Update(c_dub a_dt, c_vec3 a_cursorPosWorld, const int a_isClicked);
	void Draw(const glm::mat4& a_camProjView);

private:
	CheckersLogic* m_boardLogic;
	CheckersVisual* m_boardVisual;
	CheckersInput* m_boardInput;
};