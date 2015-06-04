#pragma once
#include <glm\glm.hpp>

class FBXModel;
class CheckersLogic;

using glm::vec3;

class CheckersVisual
{
public:
	struct Selected
	{
		TileType type;
		vec3 currPosition;
		glm::uvec2 indexOfHome;
	};
	
	CheckersVisual();
	~CheckersVisual();

	void AssembleBoardPositions();
	void BuildShaderPrograms();
	void LoadFBXFiles();
	void RenderAtMousePosition(const vec3& a_mousePos);

	void Initialise(CheckersLogic* a_logicBoard);
	void Draw(const glm::mat4& a_camProjView);
	

	const vec3 GetPositionAt(c_uint a_row, c_uint a_col);

	inline const Selected& GetSelectedPiece()
		{ return m_selectedPiece; }
	inline const void SetSelectedPiece(const Selected& a_newSelected)
		{ m_selectedPiece = a_newSelected; }

	inline const void SetPositionOfSelected(vec3 a_pos)
		{ m_selectedPiece.currPosition = a_pos; }
private:

	void RenderPieces();
	void RenderPossibleMoves();

	Selected m_selectedPiece;

	FBXModel* m_pCheckerTable;
	FBXModel* m_pCheckerPieceR;
	FBXModel* m_pCheckerPieceB;
	FBXModel* m_pPossMovePiece;

	CheckersLogic* m_pLogic;

	vec3 m_visualOfTiles[8][8];
};

