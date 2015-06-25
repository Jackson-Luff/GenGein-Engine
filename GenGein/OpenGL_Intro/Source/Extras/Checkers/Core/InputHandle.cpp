#include "LogicHandle.h"
#include "VisualHandle.h"
#include "AudioHandle.h"

#include "InputHandle.h"

using TileID = LogicHandle::TileID;

InputHandle::InputHandle()
	: m_hasBeenSelected(false)	
{}

InputHandle::~InputHandle()
{}

void InputHandle::Initialise(LogicHandle* a_pLogic,
	VisualHandle* a_pVisual, AudioHandle* a_pAudio)
{
	m_pLogic = a_pLogic;
	m_pVisual = a_pVisual;
	m_pAudio = a_pAudio;
}

const bool InputHandle::Update(const double_t a_dt,
	const f32vec3& a_cursorPosWorld, const int32_t a_isClicked)
{
	// Break Down into functions when you get back

	if (m_pLogic->TryForGameOver())
		return false;

	if (m_hasBeenSelected)
	{
		i32vec2 iOfClosest = m_pVisual->GetClosestPositionTo(a_cursorPosWorld);

		if (a_isClicked)
		{
			// Follow Cursor
			m_pVisual->SetPositionOfSelected(m_pVisual->GetPositionAt(iOfClosest));

			if (m_prevClosestIndex != iOfClosest)
			{
				m_pAudio->PlayMoveSound();
			}
		}
		else if (!a_isClicked)
		{
			// Try to place down
			if (m_pLogic->IsInPossibleMoveList(iOfClosest))
			{
				i32vec2 iOfH = m_pVisual->GetSelectedPiece().indexOfHome;
				if (m_pLogic->TryAToB(iOfH, iOfClosest))
				{
					// Standard Jump
					m_pAudio->PlayHasMovedSound();
					m_pLogic->ClearPossibleMovesList();

					if (m_pLogic->IsJumpAvaliabeAtAll(TURN_SYS::PLAYER) == false)
						return true;

					return true;
				}
			}

			m_pVisual->ResetSelectedPiece();
			m_pLogic->ClearPossibleMovesList();
			m_hasBeenSelected = false;
		}
		m_prevClosestIndex = iOfClosest;
	}
	else
		BrowsingPieces(a_cursorPosWorld, a_isClicked);

	return false;
}

//NOTE: Might put this in the visual
const bool InputHandle::BrowsingPieces(const f32vec3& a_cursorPosWorld,
	const int32_t a_isClicked)
{
	if (a_isClicked)
	{
		for (uint32_t r = 0; r < m_pLogic->m_dimCount; r++)
		{
			for (uint32_t c = 0; c < m_pLogic->m_dimCount; c++)
			{
				if (m_pLogic->GetIDAt(i32vec2(r, c)) == TileID::BLANK_BLACK  ||
					m_pLogic->GetIDAt(i32vec2(r, c)) == TileID::BLANK_RED	 || 
					m_pLogic->GetIDAt(i32vec2(r, c)) == TileID::PLAYER_BLACK ||
					m_pLogic->GetIDAt(i32vec2(r, c)) == TileID::PLAYER_BLACK_KING )
					continue;

				float32_t dist = length(a_cursorPosWorld - m_pVisual->GetPositionAt(i32vec2(r, c)));
				
				if (dist < (m_pLogic->m_tileSize/2.0f) && a_isClicked)
				{
					VisualHandle::Selected sel;
					i32vec2 cindex = i32vec2(r, c);
					sel.type = m_pLogic->GetIDAt(cindex);
					sel.indexOfHome = i32vec2(r, c);
					m_pVisual->SetSelectedPiece(sel);
					m_pLogic->GeneratePossibleMoves(cindex);
					m_hasBeenSelected = true;

					if (sel.type == TileID::PLAYER_RED)
						printf("This type is PLAYER_RED!\n");
					else if (sel.type == TileID::PLAYER_BLACK)
						printf("This type is PLAYER_BLACK!\n");
					return true;
				}
			}
		}
	}

	return false;
}