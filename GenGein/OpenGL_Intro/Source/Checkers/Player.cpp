#include <AIE/Gizmos.h>

#include "Player.h"

Player::Player(CHECKERS_DATA::PLAYER_TYPE a_type)
	: BasePlayer(a_type)
{}

Player::~Player()
{
	Gizmos::destroy();
}

void Player::ApplyPositions(const float& a_tileSize, const std::vector<glm::vec3>& a_boardData)
{
	if (a_boardData.size() <= 0) return;
	
	if (m_IDType == CHECKERS_DATA::PLAYER_01)
	{
		for (int i = 0; i < 12; i++)
		{
			CHECKERS_DATA::PieceData* piece = new CHECKERS_DATA::PieceData();
			piece->position = a_boardData[i];
			piece->colour = vec3(1, 0, 0);
			piece->type = m_IDType;
			m_pieceData.push_back(piece);
		}

		m_possiblePositions = a_boardData;
	}
	else if (m_IDType == CHECKERS_DATA::PLAYER_02)
	{
		for (uint i = a_boardData.size() - 1; i > a_boardData.size() - 13; i--)
		{
			CHECKERS_DATA::PieceData* piece = new CHECKERS_DATA::PieceData();
			piece->position = a_boardData[i];
			piece->colour = vec3(0, 0, 1);
			piece->type = m_IDType;
			m_pieceData.push_back(piece);
		}

		m_possiblePositions = a_boardData;
	}
}

void Player::Initialise()
{
	Gizmos::create();

	m_selectedPiece.homePosition = vec3(0);
	m_selectedPiece.indexOfSelected = 0;
	m_selectedPiece.isSelected = false;
	m_hasMoved = false;
}

bool Player::isValidMove()
{
	// diagonal adjacents to home position
	//m_pieceData[m_selectedPiece.indexOfSelected]
	//if (m_pieceData[m_selectedPiece.indexOfSelected]->position)
	return true;
}

void Player::MoveCheckerPiece()
{
	int indexOfClosest = 1000;
	float smallestDist = 1000.0f;
	for (uint i = 0; i < m_possiblePositions.size(); i++)
	{
		float dist = glm::length(m_possiblePositions[i] - m_pieceData[m_selectedPiece.indexOfSelected]->position);

		if (dist < smallestDist)
		{
			smallestDist = dist;
			indexOfClosest = i;
		}
	}

	m_pieceData[m_selectedPiece.indexOfSelected]->position = m_possiblePositions[indexOfClosest];
	m_pieceData[m_selectedPiece.indexOfSelected]->position.y = 0.5f;
}

void Player::Update(const double a_dt, const glm::vec3& a_posOfMouseToAxis, const int a_hasClicked)
{
	if (m_selectedPiece.isSelected)
	{
		m_pieceData[m_selectedPiece.indexOfSelected]->position = a_posOfMouseToAxis;
		
		if (a_hasClicked)
		{
			MoveCheckerPiece();
			m_hasMoved = true;
		}
		else if (isValidMove())
		{
			m_pieceData[m_selectedPiece.indexOfSelected]->position = a_posOfMouseToAxis;
			m_selectedPiece.isSelected = false;
			m_hasMoved = false;
		}
		else
		{
			m_pieceData[m_selectedPiece.indexOfSelected]->position = m_selectedPiece.homePosition;
		}

	}
	else
	{
		for (uint i = 0; i < m_pieceData.size(); i++)
		{
			float dist = glm::length(a_posOfMouseToAxis - m_pieceData[i]->position);

			if (dist < 1.0f && a_hasClicked && m_pieceData[i]->type == m_IDType)
			{
				m_selectedPiece.homePosition = m_pieceData[i]->position;
				m_selectedPiece.indexOfSelected = i;
				m_selectedPiece.isSelected = true;
			}

		}

		m_hasMoved = false;
	}
}

void Player::Draw(const glm::mat4& a_projView, const float& a_tileSize)
{
	Gizmos::clear();

	for (auto& piece : m_pieceData)
	{
		Gizmos::addCylinderFilled(piece->position,
			a_tileSize / 3.0f, a_tileSize/10.0f, 20, glm::vec4(piece->colour, 1));
	}

	Gizmos::draw(a_projView);
}