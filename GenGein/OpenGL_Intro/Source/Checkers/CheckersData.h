#pragma once
#include <glm\glm.hpp>

using glm::vec3;
using glm::vec2;

namespace CHECKERS_DATA
{
	struct PieceData
	{
		PieceData() :
		position(vec3(0)),
		colour(vec3(0)) {}

		glm::vec3 position;
		glm::vec3 colour;
	};

	struct BoardData
	{
		glm::vec3 position;
		glm::vec2 uv;
	};
}