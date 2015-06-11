#include <gl_core_4_4.h>
#include <glm\ext.hpp>
#include "Engine\Core\ShaderHandler.h"

#include "TransGrid.h"

TransGrid::TransGrid()
{
}

TransGrid::~TransGrid()
{
	delete[] verts;

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void TransGrid::SetUpShaderProgram(const_pChar a_name,
	const_pChar a_vert, const_pChar a_frag,
	const_pChar a_geo, const_pChar a_tessC, 
	const_pChar a_tessE)
{
	ShaderHandler::LoadShaderProgram(a_name,
		a_vert, a_frag,
		a_geo, a_tessC,
		a_tessE);

	m_programID = &ShaderHandler::GetShader(a_name);
}

void TransGrid::AddPerlinToGUI()
{
	m_amplitude = -30;
	m_seeder = 12;
	m_octaves = 6;
	m_persistence = 0.3f;
/*
	a_gui->AddVarRW("Main Tweaker", "Environment", "Seed", TwType::TW_TYPE_FLOAT, (void*)&m_seeder);
	a_gui->AddVarRW("Main Tweaker", "Environment", "Amplitude", TwType::TW_TYPE_FLOAT, (void*)&m_amplitude);

	a_gui->AddVarRW("Main Tweaker", "Environment", "Persistence", TwType::TW_TYPE_FLOAT, (void*)&m_persistence);
	a_gui->AddVarRW("Main Tweaker", "Environment", "Octaves", TwType::TW_TYPE_FLOAT, (void*)&m_octaves);*/
}

void TransGrid::ApplyDataToGL(const uint32_t* a_indices)
{
	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_dims * m_dims * sizeof(VertexData), verts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(uint), a_indices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
}

void TransGrid::GenSegmentedGrid(const uint32_t a_dim,
	const float32_t a_segScale,
	bool a_perlin, bool a_dimSqr)
{
	m_dims = a_dim;

	float center = (a_dim*a_segScale) * 0.5f;

	// Populate verts with row + cols input
	verts = new VertexData[a_dim * a_dim];

	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			verts[r * a_dim + c].position = glm::vec4((float)(c * a_segScale), 0,(float)(r * a_segScale), 1);
			verts[r * a_dim + c].normal = glm::vec4(0, 1, 0, 0);
			verts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* indices = new GLuint[((a_dim - 1) * (a_dim - 1) * 6)];
	m_indexCount = 0;

	for (GLuint r = 0; r < (a_dim - 1); ++r)
	{
		for (GLuint c = 0; c < (a_dim - 1); ++c)
		{
			// triangle 1
			indices[m_indexCount++] = r * a_dim + c;
			indices[m_indexCount++] = (r + 1) * a_dim + c;
			indices[m_indexCount++] = (r + 1) * a_dim + (c + 1);
			// triangle 2
			indices[m_indexCount++] = r * a_dim + c;
			indices[m_indexCount++] = (r + 1) * a_dim + (c + 1);
			indices[m_indexCount++] = r * a_dim + (c + 1);
		}
	}

	if (a_perlin)
		GenPerlin();
	else if (a_dimSqr)
		GenDimSqr();

	ApplyDataToGL(indices);

	delete[] indices;
}

void TransGrid::GenQuad(const float32_t a_scale, 
	bool a_perlin, bool a_dimSqr)
{
	m_dims = 2;

	float center = (m_dims*a_scale) * 0.5f;

	// Populate verts with row + cols input
	verts = new VertexData[m_dims * m_dims];

	for (GLuint r = 0; r < m_dims; ++r)
	{
		for (GLuint c = 0; c < m_dims; ++c)
		{
			verts[r * m_dims + c].position = glm::vec4((float)(c * a_scale), 0,(float)(r * a_scale), 1);
			verts[r * m_dims + c].normal = glm::vec4(0, 1, 0, 0);
			verts[r * m_dims + c].uv = glm::vec2((float)r / m_dims*2, (float)c / m_dims*2);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* indices = new GLuint[((m_dims - 1) * (m_dims - 1) * 6)];
	m_indexCount = 0;

	for (GLuint r = 0; r < (m_dims - 1); ++r)
	{
		for (GLuint c = 0; c < (m_dims - 1); ++c)
		{
			// triangle 1
			indices[m_indexCount++] = r * m_dims + c;
			indices[m_indexCount++] = (r + 1) * m_dims + c;
			indices[m_indexCount++] = (r + 1) * m_dims + (c + 1);
			// triangle 2
			indices[m_indexCount++] = r * m_dims + c;
			indices[m_indexCount++] = (r + 1) * m_dims + (c + 1);
			indices[m_indexCount++] = r * m_dims + (c + 1);
		}
	}

	if (a_perlin)
		GenPerlin();
	else if (a_dimSqr)
		GenDimSqr();

	ApplyDataToGL(indices);

	delete[] indices;
}

void TransGrid::GenPerlin()
{
	AddPerlinToGUI();

	for (uint r = 0; r < m_dims; r++)
	{
		for (uint c = 0; c < m_dims; c++)
		{
			// Perlin data
			float pscale = (1.0f / m_dims) * 3;
			float persistence = m_persistence;
			float ampli = m_amplitude;
			float octaves = m_octaves;

			for (float o = 0; o < octaves; ++o)
			{
				float freq = powf(0.3f, (float)o);
				float perlin_sample = glm::perlin((glm::vec2((float)r, (float)c) + m_seeder) * pscale * freq) + 0.5f;

				verts[r * m_dims + c].position.y += -m_amplitude / 10 + (perlin_sample * ampli * 2);
				ampli *= persistence;
			}
		}
	}
	
}

void TransGrid::GenDimSqr()
{

}

void TransGrid::Render()
{
	glUseProgram(*m_programID);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}