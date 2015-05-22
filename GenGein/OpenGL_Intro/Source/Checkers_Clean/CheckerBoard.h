#pragma once

#include <vector>
#include "Board.h"

using glm::vec2;
using glm::uvec2;

class CheckerBoard : public Board
{
public:
	CheckerBoard();
	~CheckerBoard();

	void Initialise();

	vec3 IndexToPos(c_uint a_row, c_uint a_col);
	vec2 PosToIndex(const vec3 a_pos);

	void Draw(const glm::mat4& a_camProjView);

private:
	void FindAndInitBlack();
	void FindAndInitWhite();

	std::vector<uvec2> m_indexOfPiecePos;
};

