#pragma once
#include <glm\glm.hpp>

class ObjMesh;
class FBXModel;
class LogicHandle;

////
// Author: Jackson Luff
// Name: VisualHandle
// Type: class (standard)
// Parent: none
// Description:
// * This class is the visual display of 
// * the checkers logic. It servers no other
// * purpose than to make checkers look 
// * really, really pretty.
////
class VisualHandle
{
public:
	// Struct of selected piece data
	struct Selected
	{
		LogicHandle::TileID type;
		glm::f32vec3 currPosition;
		glm::i32vec2 indexOfHome;
	};
	
	// Constructor
	VisualHandle();
	// Deconstructor
	~VisualHandle();

	// Initialise the board positions and model info
	void Initialise(LogicHandle* a_logicBoard);
	// Render model info to board positions
	void Render(const glm::f32mat4& a_camProjView);
	// Clears mem vars and such
	void Shutdown();

	// Returns position at index
	glm::f32vec3& GetPositionAt(const glm::i32vec2& a_cIndex);

	// Returns info ref of selected piece
	inline const Selected& GetSelectedPiece() const
		{ return m_selectedPiece; }

	// Setter of selected Piece
	inline const void SetSelectedPiece(const Selected& a_newSelected) 
		{ m_selectedPiece = a_newSelected; }

	// Set position of selector (handy for input)
	inline const void SetPositionOfSelected(const glm::f32vec3& a_pos)
		{ m_selectedPiece.currPosition = a_pos; }

	// Nullifies selected piece
	const void ResetSelectedPiece();

	// Iterates and returns closest board 
	// position relative to given position
	const i32vec2 GetClosestPositionTo( const f32vec3& a_inPos) const;

private:
	// Helper functions to clean code:
	const void AssembleBoardPositions();
	const void BuildShaderPrograms();
	const void LoadFBXFiles();
	const void RenderPieces();
	const void RenderSelectedPiece();
	const void RenderPossibleMoves(); 

	//Remember valitidity checks:
	const bool IsWithinArrayBounds(const i32vec2& a_cIndex);

	//NOTE: not clean. But eh.
	Selected m_selectedPiece;

	//NOTE: may remove seperation of
	// FBXModels for checker pieces 
	// and alter colour based on ID
	// in LogicHandle... Maybe.

	// FBX instance for table model
	FBXModel* m_pCheckerTable;
	// FBX instance for red checker piece model
	FBXModel* m_pCheckerPieceR;
	// FBX instance for black checker piece model
	FBXModel* m_pCheckerPieceB;
	// FBX instance for possible moves model
	FBXModel* m_pPossMovePiece;
	// FBX instance for move piece model
	FBXModel* m_pSelectorPiece;

	//Gameover Quad
	ObjMesh* m_gameOverQuad;

	// LogicHandle Reference(non &)
	LogicHandle* m_pLogic;

	// Position representation of logic board
	f32vec3 m_boardPositions[8][8];
};