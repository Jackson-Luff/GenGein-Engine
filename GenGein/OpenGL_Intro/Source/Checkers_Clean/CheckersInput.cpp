#include "CheckersLogic.h"
#include "CheckersVisual.h"
#include "CheckersInput.h"

CheckersInput::CheckersInput()
{}


CheckersInput::~CheckersInput()
{}

void CheckersInput::Initialise(CheckersLogic* a_pLogicBoard, CheckersVisual* a_pVisualBoard)
{
	m_pLogic = a_pLogicBoard;
	m_pVisual = a_pVisualBoard;
}

void CheckersInput::Update(c_dub a_dt, c_vec3& a_cursorPosWorld, c_int a_isClicked)
{
	if (hasBeenSelected)
	{
		glm::uvec2 iOfH = m_pVisual->GetSelectedPiece().indexOfHome;

		if (a_isClicked)
			m_pVisual->SetPositionOfSelected(a_cursorPosWorld); // Follow Cursor
		else if (!a_isClicked)
		{
			// Try to place down
			if (m_pLogic->doMove(iOfH.x, iOfH.y, iOfClosest.x, iOfClosest.y))
			{}
			else
				m_pVisual->SetPositionOfSelected(m_pVisual->GetPositionAt(iOfH.x, iOfH.y));

			m_pLogic->ClearPossibleMoves();
			hasBeenSelected = false;
			//m_isMyTurn = false;
		}
	}
	else //if (m_isMyTurn)
		BrowsingPieces(a_cursorPosWorld, a_isClicked);
}

bool CheckersInput::BrowsingPieces(c_vec3& a_cursorPosWorld, c_int a_isClicked)
{
	if (a_isClicked)
	{
		for (uint r = 0; r < m_pLogic->GetRowCount(); r++)
		{
			for (uint c = 0; c < m_pLogic->GetColCount(); c++)
			{
				float dist = glm::length(a_cursorPosWorld - m_pVisual->GetPositionAt(r, c));

				if (dist < m_pLogic->GetTileSize() && a_isClicked)
				{
					CheckersVisual::Selected sel = m_pVisual->GetSelectedPiece();
					sel.type = m_pLogic->GetPieceAt(r, c);
					sel.indexOfHome = glm::uvec2(r, c);
					m_pLogic->GenPossibleMoves(r, c);
					hasBeenSelected = true;
					return true;
				}
			}
		}
	}

	return false;
}