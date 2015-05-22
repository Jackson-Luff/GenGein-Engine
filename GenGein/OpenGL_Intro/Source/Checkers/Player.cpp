#include <AIE/Gizmos.h>

#include "Player.h"

Player::Player(CHECKERS_DATA::PIECE_TYPE a_type)
	: BasePlayer(a_type)
{}

Player::~Player()
{
	Gizmos::destroy();
}

void Player::ApplyPositions(const float& a_tileSize, const std::vector<CHECKERS_DATA::BoardTile*>& a_boardData)
{
	if (a_boardData.size() <= 0) return;
	
	if (m_IDType == CHECKERS_DATA::PLAYER_BLUE)
	{
		m_TilesOnBoard = a_boardData;
		for (int i = 0; i < 12; i++)
		{
			CHECKERS_DATA::PieceData* piece = new CHECKERS_DATA::PieceData();
			piece->position = a_boardData[i]->position;
			piece->colour = vec3(0, 0, 1);
			piece->type = m_IDType;
			m_moveablePieces.push_back(piece);
			//m_positionsOnBoard.erase(m_positionsOnBoard.begin() + i);
		}
	}
	else if (m_IDType == CHECKERS_DATA::PLAYER_RED)
	{
		m_TilesOnBoard = a_boardData;
		for (uint i = a_boardData.size() - 1; i > a_boardData.size() - 13; i--)
		{
			CHECKERS_DATA::PieceData* piece = new CHECKERS_DATA::PieceData();
			piece->position = a_boardData[i]->position;
			piece->colour = vec3(1, 0, 0);
			piece->type = m_IDType;
			m_moveablePieces.push_back(piece);
			//m_positionsOnBoard.erase(m_positionsOnBoard.begin() + i);
		}	
	}
}

void Player::Initialise()
{
	BasePlayer::Initialise();
	Gizmos::create();

	m_selectedPiece.homePosition = vec3(0);
	m_selectedPiece.index = 0;
	m_selectedPiece.isSelected = false;
	m_isMyTurn = false;
}

bool Player::CheckIfCanJump(const vec3& a_a, const vec3& a_b)
{
	uint colCount = 8;
	uint colPlusOne = colCount + 1;
	uint colMinusOne = colCount - 1;
	
	// Checks surrounding:
	// Top-Left = curr + (colCount - 1)
	// Top-rght = curr + (colCount + 1)
	// Bot-Left = curr - (colCount - 1)
	// Bot-rght = curr + (colCount + 1)

	// Checks possible jumps:
	// Top-Left = curr + ((colCount - 1)*2)
	// Top-rght = curr + ((colCount + 1)*2)
	// Bot-Left = curr - ((colCount - 1)*2)
	// Bot-rght = curr + ((colCount + 1)*2)

	return false;
}

bool Player::TryJump(const vec3& a_a, const vec3& a_b)
{


	return false;
}

bool Player::isValidMove(const CHECKERS_DATA::MOVE_TYPE& a_type)
{
	glm::vec3 target = m_TilesOnBoard[m_closestPosIndex]->position;
	glm::vec3 home = m_selectedPiece.homePosition;
	float length;

	length = target.x - home.x;

	// Player1
	if (m_IDType == CHECKERS_DATA::PLAYER_BLUE)
	{
		if (length > 0 && length < 3)
		{
			// trying to stride : trying to jump
			if (length < 2)
				return true;
			else
				return TryJump(home, target);
		}
	}

	// Player2
	if (m_IDType == CHECKERS_DATA::PLAYER_RED)
	{	
		if (length < 0 && length < 3)
		{
			// trying to stride : trying to jump
			if (length < 2)
				return true;
			else
				return TryJump(home, target);
		}
	}

	return false;

	// diagonal adjacents to home position
	//m_pieceData[m_selectedPiece.indexOfSelected]
	//if (m_pieceData[m_selectedPiece.indexOfSelected]->position)
}

void Player::MoveCheckerPiece()
{
	int indexOfClosest = 1000;
	float smallestDist = 1000.0f;
	for (uint i = 0; i < m_TilesOnBoard.size(); i++)
	{
		glm::vec3& closestOnBoard = m_TilesOnBoard[i]->position;
		glm::vec3& selected = m_moveablePieces[m_selectedPiece.index]->position;
		float dist = glm::length(closestOnBoard - selected);

		if (dist < smallestDist)
		{
			smallestDist = dist;
			indexOfClosest = i;
		}
	}
	m_closestPosIndex = indexOfClosest;

	m_moveablePieces[m_selectedPiece.index]->position = m_TilesOnBoard[m_closestPosIndex]->position;
	m_moveablePieces[m_selectedPiece.index]->position.y = 0.5f;
}

void Player::Update(const double a_dt, const glm::vec3& a_posOfMouseToAxis, const int a_hasClicked)
{
	if (m_selectedPiece.isSelected)
	{
		m_moveablePieces[m_selectedPiece.index]->position = a_posOfMouseToAxis;
		
		if (a_hasClicked)
			MoveCheckerPiece();
		else if (!a_hasClicked && isValidMove(CHECKERS_DATA::STRIDE))
		{
			m_moveablePieces[m_selectedPiece.index]->position = m_TilesOnBoard[m_closestPosIndex]->position;
			m_selectedPiece.isSelected = false;
			m_isMyTurn = false;
		}
		else if (!isValidMove(CHECKERS_DATA::STRIDE))
		{
			m_moveablePieces[m_selectedPiece.index]->position = m_selectedPiece.homePosition;
			m_selectedPiece.isSelected = false;
		}
	}
	else if (m_isMyTurn)
	{
		for (uint i = 0; i < m_moveablePieces.size(); i++)
		{
			float dist = glm::length(a_posOfMouseToAxis - m_moveablePieces[i]->position);

			if (dist < 1.0f && a_hasClicked && m_moveablePieces[i]->type == m_IDType)
			{ 
				m_selectedPiece.homePosition = m_moveablePieces[i]->position;
				m_selectedPiece.index = i;
				m_selectedPiece.isSelected = true;
			}
		}
	}
}
