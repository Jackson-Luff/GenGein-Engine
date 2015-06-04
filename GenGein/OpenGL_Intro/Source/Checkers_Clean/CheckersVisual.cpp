#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\TransGrid.h"
#include "CheckersLogic.h"
#include "CheckersVisual.h"


CheckersVisual::CheckersVisual()
{}

CheckersVisual::~CheckersVisual()
{
	delete m_pCheckerPieceR;
	delete m_pCheckerPieceB;
	delete m_pCheckerTable;
	delete m_pLogic;
}

void CheckersVisual::AssembleBoardPositions()
{
	for (uint y = 0; y < m_pLogic->GetColCount(); y++)
	{
		for (uint x = 0; x < m_pLogic->GetColCount(); x++)
		{
			float tSize = m_pLogic->GetTileSize();
			float xP = (tSize*2.95f) + -(x * tSize/1.2f);
			float zP = (tSize*2.95f) + -(y * tSize/1.2f);
			vec3 pos = vec3(xP, 7.32f, zP);
			m_visualOfTiles[x][y] = pos;
		}
	}
}

void CheckersVisual::BuildShaderPrograms()
{
	ShaderHandler::LoadShaderProgram("CheckerTable",
		"Data/Shaders/Checkers/Table.vert",
		"Data/Shaders/Checkers/Table.frag");

	ShaderHandler::LoadShaderProgram("CheckerPiece",
		"Data/Shaders/Checkers/Piece.vert",
		"Data/Shaders/Checkers/Piece.frag");

	ShaderHandler::LoadShaderProgram("PossibleMoves",
		"Data/Shaders/Checkers/PossibleMoves.vert",
		"Data/Shaders/Checkers/PossibleMoves.frag");
}

void CheckersVisual::LoadFBXFiles()
{
	m_pCheckerTable = new FBXModel();
	m_pCheckerTable->LoadFBX("CheckerTable",
		"Data/Models/Checkers/table_game.fbx",
		FBXFile::UNITS_METER);

	m_pCheckerPieceR = new FBXModel();
	m_pCheckerPieceR->LoadFBX("CheckerPiece",
		"Data/Models/Checkers/redChecker_game.fbx",
		FBXFile::UNITS_METER);

	m_pCheckerPieceB = new FBXModel();
	m_pCheckerPieceB->LoadFBX("CheckerPiece",
		"Data/Models/Checkers/blackChecker_game.fbx",
		FBXFile::UNITS_METER);

	m_pPossMovePiece = new FBXModel();
	m_pPossMovePiece->LoadFBX("PossibleMoves",
		"Data/Models/sphere.fbx",
		FBXFile::UNITS_METER);
}

void CheckersVisual::Initialise(CheckersLogic* a_logicBoard)
{
	m_pLogic = a_logicBoard;

	AssembleBoardPositions();

	BuildShaderPrograms();

	LoadFBXFiles();

	m_pCheckerTable->SetLocalTransform(glm::translate(vec3(0, 0, 0)));
}

void CheckersVisual::Draw(const glm::mat4& a_camProjView)
{
	TextureHandler::RenderAllTextures();

	m_pCheckerTable->Render();

	RenderPieces();

	RenderPossibleMoves();
}

const vec3 CheckersVisual::GetPositionAt(c_uint a_row, c_uint a_col)
{
	return m_visualOfTiles[a_row][a_col];
}

// Private Func's:

void CheckersVisual::RenderPieces()
{
	for (uint y = 0; y < m_pLogic->GetBoardSize(); y++)
	{
		for (uint x = 0; x < m_pLogic->GetBoardSize(); x++)
		{
			TileType currType = m_pLogic->GetPieceAt(x, y);

			if (currType == INVALID || currType == BLANK)
				continue;

			if (currType == PLAYER_RED)
			{
				m_pCheckerPieceR->SetLocalTransform(glm::translate(m_visualOfTiles[x][y]));
				m_pCheckerPieceR->Render();
			}
			else if (currType == PLAYER_BLUE)
			{
				m_pCheckerPieceB->SetLocalTransform(glm::translate(m_visualOfTiles[x][y]));
				m_pCheckerPieceB->Render();
			}
		}
	}
}

void CheckersVisual::RenderPossibleMoves()
{
	for (auto& i : m_pLogic->GetPossibleMoves())
	{
		vec3 pos = GetPositionAt(i.x, i.y);
		m_pPossMovePiece->SetLocalTransform(glm::translate(pos));
		m_pPossMovePiece->Render();
	}
}

void CheckersVisual::RenderAtMousePosition(const vec3& a_mousePos)
{
	m_pPossMovePiece->SetLocalTransform(glm::translate(a_mousePos));
	m_pPossMovePiece->Render();
}