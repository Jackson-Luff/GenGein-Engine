#pragma once
#include <glm\glm.hpp>

typedef const unsigned int c_uint;
typedef unsigned int uint;
typedef const int c_int;
typedef const float c_float;
typedef const char* c_pChar;

class TransGrid
{
public:
	TransGrid();
	~TransGrid();

	struct VertexData
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 uv;
	};

	// Base Functions:
	void SetUpShaderProgram(c_pChar a_name, c_pChar a_vert, c_pChar a_frag,
		c_pChar a_geo = nullptr, c_pChar a_tessC = nullptr, c_pChar a_tessE = nullptr);
	const uint GetShader() const {	return *m_programID; };

	void GenSegmentedGrid(c_uint a_dimensions, c_float a_segScale, bool a_perlin, bool a_dimSqr);
	void GenQuad(c_float a_scale, bool a_perlin, bool a_dimSqr);

	void Render();

private:
	// Send Vertex data to Buffers:
	void ApplyDataToGL(uint* a_indices);

	// Base Mods:
	void AddPerlinToGUI();
	void GenPerlin();
	void GenDimSqr();

	// Base data for outcome:
	VertexData* verts;
	uint m_indexCount;
	uint m_VAO, m_VBO, m_IBO;
	uint* m_programID;

	// Perlin private members:
	uint m_dims;
	float m_persistence;
	float m_amplitude;
	float m_octaves;
	float m_seeder;

	// Diamond-Square private members:

};

