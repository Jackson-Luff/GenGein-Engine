#pragma once
#include <glm\glm.hpp>

class CheckersLogic;

typedef const double c_dub;
typedef const vec3 c_vec3;
class CheckersInput
{
public:
	CheckersInput();
	~CheckersInput();

	void Initialise(CheckersLogic* a_pLogicBoard, CheckersVisual* a_pVisualBoard);
	void Update(c_dub a_dt, c_vec3 a_cursorPosWorld, const int a_isClicked);

	bool BrowsingPieces(c_vec3 a_cursorPosWorld);
	
private:
	CheckersLogic* m_pLogic;
	CheckersVisual* m_pVisual;

	bool hasBeenSelected;
	glm::uvec2 iOfClosest;
};