#pragma once
#include <glm\glm.hpp>

class CheckersLogic;

typedef const int c_int;
typedef const double c_dub;
typedef const vec3 c_vec3;

class CheckersInput
{
public:
	CheckersInput();
	~CheckersInput();

	void Initialise(CheckersLogic* a_pLogicBoard, CheckersVisual* a_pVisualBoard);
	void Update(c_dub a_dt, c_vec3& a_cursorPosWorld, c_int a_isClicked);

	bool BrowsingPieces(c_vec3& a_cursorPosWorld, c_int a_isClicked);

private:
	CheckersLogic* m_pLogic;
	CheckersVisual* m_pVisual;

	bool hasBeenSelected;
	glm::uvec2 iOfClosest;
};