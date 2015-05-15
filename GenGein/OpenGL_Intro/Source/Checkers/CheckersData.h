#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;

using glm::vec3;
using glm::vec2;

namespace CHECKERS_DATA
{
	enum PLAYER_TYPE
	{
		PLAYER_01 = 0,
		PLAYER_02 = 1,
		AI_COMPONENT = 2
	};

	struct PieceData
	{
		PieceData() :
		position(vec3(0)),
		colour(vec3(0)) {}

		glm::vec3 position;
		glm::vec3 colour;
		PLAYER_TYPE type;
	};

	struct BoardData
	{
		glm::vec3 position;
		glm::vec2 uv;
	};

	struct SelectedPiece
	{
		int indexOfSelected;
		bool isSelected;
		glm::vec3 homePosition;
	};
}