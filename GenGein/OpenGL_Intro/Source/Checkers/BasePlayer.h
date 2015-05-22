#pragma once
#include <vector>
#include <glm\glm.hpp>
#include "CheckersData.h"

using glm::vec3;
using glm::vec2;

class BasePlayer
{
public:
	BasePlayer(CHECKERS_DATA::PIECE_TYPE a_type);
	~BasePlayer();

	void Initialise();
	void RemovePieceAtPosition(const glm::vec3& a_pos);
	void RemovePieceAtIndex(const uint& a_index);
	void Update(const double a_dt);
	void Draw(const glm::mat4& a_projView);
protected:
	CHECKERS_DATA::PIECE_TYPE m_IDType;
	std::vector< CHECKERS_DATA::PieceData* > m_moveablePieces;
	std::vector< CHECKERS_DATA::PieceData* > m_capturedPieces;
};