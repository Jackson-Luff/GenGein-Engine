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

void CheckersInput::Update(c_dub a_dt, c_vec3 a_cursorPosWorld, const int a_isClicked)
{
	if (hasBeenSelected)
	{
		if (a_isClicked)
			m_pVisual->SetPositionOfSelected(a_cursorPosWorld); // Follow Cursor
		else if (!a_isClicked)
		{
			// Try to place down
			glm::uvec2 iOfH = m_pVisual->GetSelectedPiece().indexOfHome;
			if(m_pLogic->doMove(iOfH.x, iOfH.y, iOfClosest.x, iOfClosest.y))
			

				//Move
			
			
			//...
			//else
			//	set selected at home position, try selecting again

			hasBeenSelected = false;
			//m_isMyTurn = false;
		}
	}
	else //if (m_isMyTurn)
	{
		BrowsingPieces(a_cursorPosWorld);
	}
}