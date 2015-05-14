#pragma once

#include "BasePlayer.h"

class Player : public BasePlayer
{
public:
	Player();
	~Player();

	
	void Initialise();
	void ApplyPositions(const float& a_tileSize, const std::vector<glm::vec3>& a_boardData);

	void Update(const double a_dt);
	void Draw(const glm::mat4& a_projView);
private:
	
};