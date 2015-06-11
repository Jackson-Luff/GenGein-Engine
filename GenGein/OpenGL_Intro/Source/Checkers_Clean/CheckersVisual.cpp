#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\TransGrid.h"
#include "CheckersLogic.h"
#include "CheckersVisual.h"

using TileID = CheckersLogic::TileID;

CheckersVisual::CheckersVisual()
{ /*NOTE: declare variables*/ }

CheckersVisual::~CheckersVisual()
{
	delete m_pPossMovePiece;
	delete m_pCheckerPieceR;
	delete m_pCheckerPieceB;
	delete m_pCheckerTable;
}

void CheckersVisual::Initialise(CheckersLogic* a_logicBoard)
{
	m_pLogic = a_logicBoard;

	AssembleBoardPositions();

	BuildShaderPrograms();

	LoadFBXFiles();

	m_pCheckerTable->SetLocalTransform(glm::translate(f32vec3(0, 0, 0)));
}

void CheckersVisual::Render(const glm::mat4& a_camProjView)
{
	TextureHandler::RenderAllTextures();

	m_pCheckerTable->Render();

	RenderPieces();

	RenderSelectedPiece();

	RenderPossibleMoves();
}

f32vec3& CheckersVisual::GetPositionAt(const i32vec2& a_cIndex)
{
	if (!IsWithinArrayBounds(a_cIndex))
		printf("Get Position out of array index. \n");

	return m_boardPositions[a_cIndex.x][a_cIndex.y];
}

const void CheckersVisual::ResetSelectedPiece()
{
	CheckersVisual::Selected sel;
	sel.type = CheckersLogic::TileID::BLACK;
	sel.indexOfHome = glm::i32vec2(NULL);
	sel.currPosition = glm::f32vec3(NULL);
	SetSelectedPiece(sel);
}

const i32vec2 CheckersVisual::GetClosestPositionTo(const f32vec3& a_inPos) const
{
	for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
		{
			if (m_pLogic->GetIDAt(i32vec2(r, c)) == TileID::BLACK)
				continue;

			float32_t dist = glm::length(a_inPos - m_boardPositions[r][c]);

			if (dist < m_pLogic->m_tileSize)
			{
				return i32vec2(r, c);
			}
		}
	}
	return i32vec2(0);
}

// Private Func's:

const void CheckersVisual::AssembleBoardPositions()
{
	for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
		{
			float32_t tSize = m_pLogic->m_tileSize;
			float32_t xP = (tSize*2.95f) + -(r * tSize / 1.2f);
			float32_t zP = (tSize*2.95f) + -(c * tSize / 1.2f);
			f32vec3 pos = f32vec3(xP, 0.0f, zP);
			m_boardPositions[r][c] = pos;
		}
	}
}

const void CheckersVisual::BuildShaderPrograms()
{
	ShaderHandler::LoadShaderProgram("CheckerTable",
		"Data/Shaders/Checkers/Table.vert",
		"Data/Shaders/Checkers/Table.frag");

	ShaderHandler::LoadShaderProgram("RedCheckerPiece",
		"Data/Shaders/Checkers/RedPiece.vert",
		"Data/Shaders/Checkers/RedPiece.frag");

	ShaderHandler::LoadShaderProgram("BlackCheckerPiece",
		"Data/Shaders/Checkers/BlackPiece.vert",
		"Data/Shaders/Checkers/BlackPiece.frag");

	ShaderHandler::LoadShaderProgram("PossibleMoves",
		"Data/Shaders/Checkers/PossibleMoves.vert",
		"Data/Shaders/Checkers/PossibleMoves.frag");
}

const void CheckersVisual::LoadFBXFiles()
{
	m_pCheckerTable = new FBXModel();
	m_pCheckerTable->LoadFBX("CheckerTable",
		"Data/Models/Checkers/table_game.fbx",
		FBXFile::UNITS_METER);

	m_pCheckerPieceR = new FBXModel();
	m_pCheckerPieceR->LoadFBX("RedCheckerPiece",
		"Data/Models/Checkers/redChecker_game.fbx",
		FBXFile::UNITS_METER);

	m_pCheckerPieceB = new FBXModel();
	m_pCheckerPieceB->LoadFBX("BlackCheckerPiece",
		"Data/Models/Checkers/blackChecker_game.fbx",
		FBXFile::UNITS_METER);

	m_pPossMovePiece = new FBXModel();
	m_pPossMovePiece->LoadFBX("PossibleMoves",
		"Data/Models/sphere.fbx",
		FBXFile::UNITS_METER);
}

const void CheckersVisual::RenderPieces()
{
	for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
		{
			TileID cType = m_pLogic->GetIDAt(i32vec2(r,c));

			if (cType == TileID::BLACK ||
				cType == TileID::WHITE)
				continue;

			if (cType == TileID::PLAYER_RED)
			{
				m_pCheckerPieceR->SetLocalTransform(glm::translate(m_boardPositions[r][c]));
				m_pCheckerPieceR->Render();
			}
			else if (cType == TileID::PLAYER_BLACK)
			{
				m_pCheckerPieceB->SetLocalTransform(glm::translate(m_boardPositions[r][c]));
				m_pCheckerPieceB->Render();
			}
		}
	}
}

const void CheckersVisual::RenderSelectedPiece()
{
	if (m_selectedPiece.type == TileID::PLAYER_RED ||
		m_selectedPiece.type == TileID::PLAYER_BLACK)
	{
		f32vec3 pos = m_selectedPiece.currPosition;
		pos.y = m_selectedPiece.currPosition.y + 0.1f;
		m_pPossMovePiece->SetLocalTransform(glm::translate(pos) * glm::scale(f32vec3(0.1f)));
		m_pPossMovePiece->Render();
	}
}

const void CheckersVisual::RenderPossibleMoves()
{
	for (auto& i : m_pLogic->GetPossibleMovesList())
	{
		f32mat4 mvp = glm::translate(GetPositionAt(i)) * glm::scale(f32vec3(0.1f));
		m_pPossMovePiece->SetLocalTransform(mvp);
		m_pPossMovePiece->Render();
	}
}

const bool CheckersVisual::IsWithinArrayBounds(const i32vec2& a_cIndex)
{
	if (a_cIndex.x >= 0 &&
		a_cIndex.x < m_pLogic->m_dimCount &&
		a_cIndex.y >= 0 &&
		a_cIndex.y < m_pLogic->m_dimCount)
		return true;

	return false;
}