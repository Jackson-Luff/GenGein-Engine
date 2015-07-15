#pragma once
#include <glm\glm.hpp>

typedef const char* const_pChar;

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
	void SetUpShaderProgram(const_pChar a_name,
		const_pChar a_vert,
		const_pChar a_frag,
		const_pChar a_geo = nullptr,
		const_pChar a_tessC = nullptr,
		const_pChar a_tessE = nullptr);

	const uint32_t* GetShader() const
		{ return m_programID; };

	void GenSegmentedGrid(const unsigned int a_dimensions, const float a_segScale,
		bool a_perlin = false ,
		bool a_dimSqr = false);

	void GenQuad(const float a_scale,
		bool a_perlin = false,
		bool a_dimSqr = false);

	void Render();

private:
	// Send Vertex data to Buffers:
	void ApplyDataToGL(const unsigned int* a_indices);

	// Base Mods:
	void AddPerlinToGUI();
	void GenPerlin();
	void GenDimSqr();

	// Base data for outcome:
	VertexData* verts;
	unsigned int m_indexCount;
	unsigned int m_VAO, m_VBO, m_IBO;
	unsigned int* m_programID;

	// Perlin private members:
	unsigned int m_dims;
	float m_persistence;
	float m_amplitude;
	float m_octaves;
	float m_seeder;

	// Diamond-Square private members:

};

