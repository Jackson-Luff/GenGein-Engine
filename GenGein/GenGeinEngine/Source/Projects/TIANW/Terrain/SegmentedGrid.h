#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
class SegmentedGrid
{
public:
	struct VertexData
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec2 uv;
	};

	SegmentedGrid();
	~SegmentedGrid();

	void GenSegGrid(unsigned int a_dims, float a_dimScale);
	void GenPerlinWithNormals(unsigned int a_dims, float a_dimScale);
	void ApplyMapToBuffers();
	
	void Render(unsigned int* a_programID);

private:
	// Base data
	VertexData* m_vertices;
	unsigned int* m_indices;
	unsigned int m_indexCount;
	unsigned int m_dims;

	// Buffer information
	unsigned int m_VAO, m_VBO, m_IBO;
};