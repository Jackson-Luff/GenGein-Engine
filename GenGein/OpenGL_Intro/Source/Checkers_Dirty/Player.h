//#pragma once
//
//#include "BasePlayer.h"
//
//class Player : public BasePlayer
//{
//public:
//	Player(CHECKERS_DATA::PIECE_TYPE a_type);
//	~Player();
//
//
//	void Initialise();
//	void ApplyPositions(const float& a_tileSize, 
//		const std::vector<CHECKERS_DATA::BoardTile*>& a_boardData);
//
//	bool CheckIfCanJump(const vec3& a_a, const vec3& a_b);
//	bool TryJump(const vec3& a_a, const vec3& a_b);
//	bool isValidMove(const CHECKERS_DATA::MOVE_TYPE& a_type);
//	void MoveCheckerPiece();
//	void Update(const double a_dt, const glm::vec3& a_posOfMouseToAxis, const int a_hasClicked);
//	
//	bool m_isMyTurn;
//private:
//	uint m_closestPosIndex;
//	std::vector<CHECKERS_DATA::BoardTile*> m_TilesOnBoard;
//	std::vector<glm::vec3> m_possibleMoves;
//	CHECKERS_DATA::SelectedPiece m_selectedPiece;
//};