#include "AIE\Gizmos.h"
#include "Engine\TransGrid.h"
#include "CheckersLogic.h"
#include "CheckersVisual.h"


CheckersVisual::CheckersVisual()
{}

CheckersVisual::~CheckersVisual()
{
	Gizmos::destroy();
}

void CheckersVisual::Initialise(CheckersLogic* a_logicBoard)
{
	m_pLogic = a_logicBoard;

	Gizmos::create();
	AssembleBoardPositions();

	m_boardBase = new TransGrid();
	m_boardBase->GenQuad((float)m_pLogic->GetRowCount());
	m_boardBase->SetUpShaderProgram("CheckerBoardBase",
		"Data/Shaders/Checkers/Board.vert",
		"Data/Shaders/Checkers/Board.frag");
}

void CheckersVisual::Draw(const glm::mat4& a_camProjView)
{
	Gizmos::clear();

	m_boardBase->Render();

	for (uint y = 0; y < m_pLogic->GetBoardSize(); y++)
	{
		for (uint x = 0; x < m_pLogic->GetBoardSize(); x++)
		{
			TileType currType = m_pLogic->GetPieceAt(x,y);

			if (currType == INVALID || currType == BLANK)
				continue;

			glm::vec4 colour;

			if (currType == PLAYER_RED)
				colour = glm::vec4(1, 0, 0, 1);
			else if (currType == PLAYER_BLUE)
				colour = glm::vec4(0, 0, 1, 1);

			Gizmos::addCylinderFilled(m_visualOfTiles[x][y],
				0.3f, 0.1f, 20, colour);
		}
	}
	
	Gizmos::draw(a_camProjView);
}

void CheckersVisual::AssembleBoardPositions()
{
	for (uint y = 0; y < m_pLogic->GetColCount(); y++)
	{
		for (uint x = 0; x < m_pLogic->GetColCount(); x++)
		{
			uint tSize = m_pLogic->GetTileSize();
			float xP = (tSize / 2.0f) + (x * tSize);
			float zP = (tSize / 2.0f) + (y * tSize);
			vec3 pos = vec3(xP, 0.0f, zP);
			m_visualOfTiles[x][y] = pos;
		}
	}
}

const vec3 CheckersVisual::GetPositionAt(c_uint a_row, c_uint a_col)
{
	return m_visualOfTiles[a_row][a_col];
}