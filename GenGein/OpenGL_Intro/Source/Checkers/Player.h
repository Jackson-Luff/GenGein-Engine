#pragma once

#include "BasePlayer.h"

class Player : public BasePlayer
{
public:
	Player(CHECKERS_DATA::PLAYER_TYPE a_type);
	~Player();

	
	void Initialise();
	void ApplyPositions(const float& a_tileSize, const std::vector<glm::vec3>& a_boardData);

	bool isValidMove();
	void MoveCheckerPiece();
	void Update(const double a_dt, const glm::vec3& a_posOfMouseToAxis, const int a_hasClicked);
	void Draw(const glm::mat4& a_projView);

	bool m_hasMoved;
private:
	std::vector<glm::vec3> m_possiblePositions;
	CHECKERS_DATA::SelectedPiece m_selectedPiece;
};