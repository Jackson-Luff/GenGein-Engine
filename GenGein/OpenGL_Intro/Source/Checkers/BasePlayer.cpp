#include "AIE\Gizmos.h"
#include "BasePlayer.h"

BasePlayer::BasePlayer(CHECKERS_DATA::PIECE_TYPE a_type)
{
	m_IDType = a_type;
}


BasePlayer::~BasePlayer()
{

}

void BasePlayer::Initialise()
{
	Gizmos::create();
}

void BasePlayer::RemovePieceAtPosition(const glm::vec3& a_pos)
{
	for (uint i = 0; i < m_moveablePieces.size(); i++)
	{
		if (m_moveablePieces[i]->position == a_pos)
		{
			m_moveablePieces.erase(m_moveablePieces.begin() + i);
			return;
		}	
	}
}

void BasePlayer::RemovePieceAtIndex(const uint& a_index)
{
	m_moveablePieces.erase(m_moveablePieces.begin() + a_index);
}

void BasePlayer::Draw(const glm::mat4& a_projView)
{
	Gizmos::clear();

	for (auto& piece : m_moveablePieces)
	{
		Gizmos::addCylinderFilled(piece->position,
			0.3f, 0.1f, 20, glm::vec4(piece->colour, 1));
	}

	Gizmos::draw(a_projView);
}