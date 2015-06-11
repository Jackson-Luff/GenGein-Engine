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
		f32vec4 position;
		f32vec4 normal;
		f32vec2 uv;
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

	void GenSegmentedGrid(const uint32_t a_dimensions, const float32_t a_segScale,
		bool a_perlin = false ,
		bool a_dimSqr = false);

	void GenQuad(const float32_t a_scale,
		bool a_perlin = false,
		bool a_dimSqr = false);

	void Render();

private:
	// Send Vertex data to Buffers:
	void ApplyDataToGL(const uint32_t* a_indices);

	// Base Mods:
	void AddPerlinToGUI();
	void GenPerlin();
	void GenDimSqr();

	// Base data for outcome:
	VertexData* verts;
	uint32_t m_indexCount;
	uint32_t m_VAO, m_VBO, m_IBO;
	uint32_t* m_programID;

	// Perlin private members:
	uint32_t m_dims;
	float m_persistence;
	float m_amplitude;
	float m_octaves;
	float m_seeder;

	// Diamond-Square private members:

};

