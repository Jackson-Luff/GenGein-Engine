#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\Objects\ObjMesh.h"
#include "Engine\TransGrid.h"
#include "LogicHandle.h"
#include "VisualHandle.h"

using TileID = LogicHandle::TileID;

VisualHandle::VisualHandle()
{ /*NOTE: declare variables*/ }

VisualHandle::~VisualHandle()
{}

void VisualHandle::Initialise(LogicHandle* a_logicBoard)
{
	m_pLogic = a_logicBoard;

	AssembleBoardPositions();

	BuildShaderPrograms();

	LoadFBXFiles();

	//m_gameOverQuad = new ObjMesh();
	//m_gameOverQuad->LoadObject("Data/Objects/Plane", "plane", "Data/Object/Plane/gameOver.png");

	m_pCheckerTable->SetLocalTransform(glm::translate(f32vec3(0, 0, 0)));
}

void VisualHandle::Render(const glm::mat4& a_camProjView)
{
	TextureHandler::RenderAllTextures();

	m_pCheckerTable->Render();

	RenderPieces();

	RenderSelectedPiece();

	RenderPossibleMoves();
}

void VisualHandle::Shutdown()
{
	delete m_pPossMovePiece;
	delete m_pCheckerPieceR;
	delete m_pCheckerPieceB;
	delete m_pCheckerTable;
	//delete m_gameOverQuad;
}

f32vec3& VisualHandle::GetPositionAt(const i32vec2& a_cIndex)
{
	if (!IsWithinArrayBounds(a_cIndex))
		printf("Get Position out of array index. \n");

	return m_boardPositions[a_cIndex.x][a_cIndex.y];
}

const void VisualHandle::ResetSelectedPiece()
{
	VisualHandle::Selected sel;
	sel.type = LogicHandle::TileID::BLANK_BLACK;
	sel.indexOfHome = glm::u32vec2(-3987655);
	sel.currPosition = glm::f32vec3(-3987655);
	SetSelectedPiece(sel);
}

const i32vec2 VisualHandle::GetClosestPositionTo(const f32vec3& a_inPos) const
{
	for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
		{
			if (m_pLogic->GetIDAt(i32vec2(r, c)) == TileID::BLANK_BLACK)
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

const void VisualHandle::AssembleBoardPositions()
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

const void VisualHandle::BuildShaderPrograms()
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

	ShaderHandler::LoadShaderProgram("SelectorPiece",
		"Data/Shaders/Checkers/PossibleMoves.vert",
		"Data/Shaders/Checkers/PossibleMoves.frag");
}

const void VisualHandle::LoadFBXFiles()
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
		"Data/Models/Checkers/selector_Game.fbx",
		FBXFile::UNITS_METER);

	m_pSelectorPiece = new FBXModel();
	m_pSelectorPiece->LoadFBX("SelectorPiece",
		"Data/Models/sphere.fbx",
		FBXFile::UNITS_METER);
}

const void VisualHandle::RenderPieces()
{
	for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
	{
		for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
		{
			TileID cType = m_pLogic->GetIDAt(i32vec2(r,c));

			if (cType == TileID::BLANK_BLACK ||
				cType == TileID::BLANK_RED)
				continue;

			f32vec3 pos = m_boardPositions[r][c];

			if (cType == TileID::PLAYER_RED)
			{
				m_pCheckerPieceR->SetLocalTransform(glm::translate(pos));
				m_pCheckerPieceR->Render();
			}
			else if (cType == TileID::PLAYER_BLACK)
			{
				m_pCheckerPieceB->SetLocalTransform(glm::translate(pos));
				m_pCheckerPieceB->Render();
			}
			else if (cType == TileID::PLAYER_RED_KING)
			{
				for (int32_t i = 0; i < 2; i++)
				{
					pos.y += i * 0.05f;
					m_pCheckerPieceR->SetLocalTransform(glm::translate(pos));
					m_pCheckerPieceR->Render();
				}
			}
			else if (cType == TileID::PLAYER_BLACK_KING)
			{
				for (int32_t i = 0; i < 2; i++)
				{
					pos.y += i * 0.05f;
					m_pCheckerPieceB->SetLocalTransform(glm::translate(pos));
					m_pCheckerPieceB->Render();
				}
			}
		}
	}
}

const void VisualHandle::RenderSelectedPiece()
{
	if (m_selectedPiece.type == TileID::PLAYER_RED ||
		m_selectedPiece.type == TileID::PLAYER_RED_KING ||
		m_selectedPiece.type == TileID::PLAYER_BLACK ||
		m_selectedPiece.type == TileID::PLAYER_BLACK_KING )
	{
		f32vec3 pos = m_selectedPiece.currPosition;
		pos.y = m_selectedPiece.currPosition.y + 0.2f;
		m_pSelectorPiece->SetLocalTransform(glm::translate(pos) * glm::scale(vec3(0.1f)));
		m_pSelectorPiece->Render();
	}
}

const void VisualHandle::RenderPossibleMoves()
{
	for (auto& i : m_pLogic->GetPossibleMovesList())
	{
		f32mat4 mvp = glm::translate(GetPositionAt(i));
		m_pPossMovePiece->SetLocalTransform(mvp);
		m_pPossMovePiece->Render();
	}
}

const bool VisualHandle::IsWithinArrayBounds(const i32vec2& a_cIndex)
{
	//Check if index exceeds limit
	if (a_cIndex.x >= 0 && a_cIndex.x < m_pLogic->m_dimCount &&
		a_cIndex.y >= 0 && a_cIndex.y < m_pLogic->m_dimCount)
		return true;
	 
	return false;
}