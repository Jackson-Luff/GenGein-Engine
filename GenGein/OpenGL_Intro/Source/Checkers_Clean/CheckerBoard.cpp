#include "AIE\Gizmos.h"
#include "CheckerBoard.h"

CheckerBoard::CheckerBoard()
	: Board()
{}

CheckerBoard::~CheckerBoard()
{}

void CheckerBoard::FindAndInitBlack()
{

}

void CheckerBoard::FindAndInitWhite()
{

}

void CheckerBoard::Initialise()
{
	Board::Initialise();
	Gizmos::create();

	uint rC = GetRowCount();
	uint cC = GetColCount();

	for (uint i = 0; i < rC / 2; i++)
	{
		for (uint j = 0; j < cC; j++)
		{
			if (GetPieceAtIndex(i, j).type == INVALID) continue;

			AddPieceAtIndex(PLAYER_BLUE, i, j);
			m_indexOfPiecePos.push_back(vec2(i, j));
		}
	}

	for (uint i = rC; i > rC / 2; i--)
	{
		for (uint j = 0; j < cC; j++)
		{
			if (GetPieceAtIndex(i, j).type == INVALID) continue;

			AddPieceAtIndex(PLAYER_RED, i, j);
			m_indexOfPiecePos.push_back(vec2(i, j));
		}
	}

	for (uint i = 0; i < rC; i++)
	{
		for (uint j = 0; j < cC; j ++)
		{
			printf(" %d ", GetPieceAtIndex(i, j).type);
		}
		printf("\n");
	}
	printf("\n");
}

void CheckerBoard::Draw(const glm::mat4& a_camProjView)
{
	Gizmos::clear();

	for (auto& r : m_indexOfPiecePos)
	{
		glm::vec4 colour;

		TileData& piece = GetPieceAtIndex(r.x, r.y);

		if (piece.type == PLAYER_RED)
			colour = glm::vec4(1, 0, 0, 1);
		else if (piece.type == PLAYER_BLUE)
			colour = glm::vec4(0, 0, 1, 1);

		Gizmos::addCylinderFilled(piece.position,
			0.3f, 0.1f, 20, colour);
	}

	Gizmos::draw(a_camProjView);
}