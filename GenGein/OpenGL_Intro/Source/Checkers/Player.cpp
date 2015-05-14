#include <AIE/Gizmos.h>

#include "Player.h"

Player::Player() 
	: BasePlayer()
{
	
}

Player::~Player()
{
	Gizmos::destroy();
}

void Player::ApplyPositions(const float& a_tileSize, const std::vector<glm::vec3>& a_boardData)
{
	if (a_boardData.size() <= 0) return;

	for (int i = 0; i < 16; i++)
	{		
		CHECKERS_DATA::PieceData* piece = new CHECKERS_DATA::PieceData();
		piece->position = a_boardData[i];
		piece->colour = vec3(1,0,0);
		m_pieceData.push_back(piece);
	}
}

void Player::Initialise()
{
	Gizmos::create();
}

void Player::Update(const double a_dt)
{

}

void Player::Draw(const glm::mat4& a_projView)
{
	Gizmos::clear();

	for (auto& piece : m_pieceData)
	{
		Gizmos::addCylinderFilled(piece->position,
			0.25, 0.25, 50, glm::vec4(piece->colour, 1));
	}

	Gizmos::draw(a_projView);
}