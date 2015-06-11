#include "CheckersLogic.h"
#include "CheckersVisual.h"
#include "CheckersInput.h"

using TileID = CheckersLogic::TileID;

CheckersInput::CheckersInput()
	: m_hasBeenSelected(false)	
{}

CheckersInput::~CheckersInput()
{}

void CheckersInput::Initialise(CheckersLogic* a_pLogicBoard,
	CheckersVisual* a_pVisualBoard)
{
	m_pLogic = a_pLogicBoard;
	m_pVisual = a_pVisualBoard;
}

void CheckersInput::Update(const double_t a_dt,
	const f32vec3& a_cursorPosWorld, const int32_t a_isClicked)
{
	// Break Down into functions when you get back

	if (m_hasBeenSelected)
	{
		i32vec2 iOfClosest = m_pVisual->GetClosestPositionTo(a_cursorPosWorld);

		if (a_isClicked)
			m_pVisual->SetPositionOfSelected(m_pVisual->GetPositionAt(iOfClosest)); // Follow Cursor
		else if (!a_isClicked)
		{
			// Try to place down
			if (m_pLogic->isInPossibleMoveList(iOfClosest))
			{
				i32vec2 iOfH = m_pVisual->GetSelectedPiece().indexOfHome;
				m_pLogic->TryAToB(iOfH, iOfClosest);
			}
			else
				m_pVisual->ResetSelectedPiece();

			m_pLogic->ClearPossibleMovesList();
			m_hasBeenSelected = false;
		}
	}
	else
		BrowsingPieces(a_cursorPosWorld, a_isClicked);
}

//NOTE: Might put this in the visual
const bool CheckersInput::BrowsingPieces(const f32vec3& a_cursorPosWorld,
	const int32_t a_isClicked)
{
	if (a_isClicked)
	{
		for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
		{
			for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
			{
				if (m_pLogic->GetIDAt(i32vec2(r,c)) == TileID::BLACK)
					continue;

				float32_t dist = length(a_cursorPosWorld - m_pVisual->GetPositionAt(i32vec2(r, c)));
				
				if (dist < m_pLogic->m_tileSize && a_isClicked)
				{
					CheckersVisual::Selected sel;
					i32vec2 cindex = i32vec2(r, c);
					sel.type = m_pLogic->GetIDAt(cindex);
					sel.indexOfHome = i32vec2(r, c);
					m_pVisual->SetSelectedPiece(sel);
					m_pLogic->GeneratePossibleMoves(cindex);
					m_hasBeenSelected = true;
					return true;
				}
			}
		}
	}

	return false;
}