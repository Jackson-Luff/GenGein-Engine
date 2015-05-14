#pragma once
#include <vector>
#include <glm\glm.hpp>
#include "CheckersData.h"

using glm::vec3;
using glm::vec2;

class BasePlayer
{
public:
	BasePlayer();
	~BasePlayer();

protected:

	std::vector< CHECKERS_DATA::PieceData* > m_pieceData;
};