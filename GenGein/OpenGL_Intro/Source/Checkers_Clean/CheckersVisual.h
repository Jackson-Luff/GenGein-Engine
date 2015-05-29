#pragma once
#include <glm\glm.hpp>

class TransGrid;
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

	void Initialise(CheckersLogic* a_logicBoard);
	void Draw(const glm::mat4& a_camProjView);
	void AssembleBoardPositions();

	const vec3 GetPositionAt(c_uint a_row, c_uint a_col);

	inline const Selected& GetSelectedPiece()
		{ return m_selectedPiece; }
	inline const void SetSelectedPiece(const Selected& a_newSelected)
		{ m_selectedPiece = a_newSelected; }

	inline const void SetPositionOfSelected(vec3 a_pos)
		{ m_selectedPiece.currPosition = a_pos; }
private:
	Selected m_selectedPiece;

	TransGrid* m_boardBase;
	CheckersLogic* m_pLogic;
	vec3 m_visualOfTiles[8][8];
};

