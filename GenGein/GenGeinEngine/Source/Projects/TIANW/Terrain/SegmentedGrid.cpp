#include <gl_core_4_4.h>
#include <glm\gtc\noise.hpp>

#include "SegmentedGrid.h"

SegmentedGrid::SegmentedGrid() :
	m_vertices(0), m_indices(0),
	m_indexCount(0), m_dims(0),
	m_VAO(0), m_VBO(0), m_IBO(0)
{
}

SegmentedGrid::~SegmentedGrid()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void SegmentedGrid::GenSegGrid(unsigned int a_dims, float a_dimScale)
{
	m_dims = a_dims;

	float center = (m_dims*a_dimScale) / 2.0f;

	// Populate verts with row + cols input
	m_vertices = new VertexData[m_dims * m_dims];

	for (unsigned int r = 0; r < m_dims; ++r)
	{
		for (unsigned int c = 0; c < m_dims; ++c)
		{
			m_vertices[r * m_dims + c].position = glm::vec4((float)(c * a_dimScale), 0, (float)(r * a_dimScale), 1.0f);
			m_vertices[r * m_dims + c].normal = glm::vec4(0, 1, 0, 1);
			m_vertices[r * m_dims + c].uv = glm::vec2((float)r / m_dims, (float)c / m_dims);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	m_indices = new unsigned int[((m_dims - 1) * (m_dims - 1) * 6)];
	m_indexCount = 0;

	for (unsigned int r = 0; r < (m_dims - 1); ++r)
	{
		for (unsigned int c = 0; c < (m_dims - 1); ++c)
		{
			// triangle 1
			m_indices[m_indexCount++] = r * m_dims + c;
			m_indices[m_indexCount++] = (r + 1) * m_dims + c;
			m_indices[m_indexCount++] = (r + 1) * m_dims + (c + 1);
			// triangle 2
			m_indices[m_indexCount++] = r * m_dims + c;
			m_indices[m_indexCount++] = (r + 1) * m_dims + (c + 1);
			m_indices[m_indexCount++] = r * m_dims + (c + 1);
		}
	}

}

void SegmentedGrid::GenPerlinWithNormals(unsigned int a_dims, float a_dimScale)
{
	m_dims = a_dims;

	float center = (a_dims*a_dimScale) / 2.0f;

	// Populate verts with row + cols input
	m_vertices = new VertexData[a_dims * a_dims];

	float scale = (1.0f / a_dims) * 6.0f;
	int octaves = 9;

	for (unsigned int r = 0; r < a_dims; ++r)
	{
		for (unsigned int c = 0; c < a_dims; ++c)
		{
			float amplitude = 1.f;
			float persistence = 0.2f;

			m_vertices[r * a_dims + c].position = glm::vec4((float)(c * a_dimScale), 0, (float)(r * a_dimScale), 1.0f);
			m_vertices[r * a_dims + c].uv = glm::vec2((float)c / a_dims, (float)r / a_dims);

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(4, (float)o);
				float simplex_sample = glm::simplex(glm::vec2((float)r, (float)c) * scale * freq/10.0f) * 0.5f + 0.5f;
				float perlin_sample = glm::perlin(glm::vec2((float)r, (float)c) * scale * freq) * 0.5f + 0.5f;

				m_vertices[r * a_dims + c].position.y += (perlin_sample+simplex_sample) * amplitude;
				amplitude *= persistence;
			}

			float mapGeneralScale = 5.0f;
			float mapHeightScale = ((a_dims * 150.0f) / 512.0f) * (a_dimScale/3.0f);
			m_vertices[r * a_dims + c].position.y *= mapHeightScale*mapGeneralScale;
			m_vertices[r * a_dims + c].position.x *= mapGeneralScale;
			m_vertices[r * a_dims + c].position.z *= mapGeneralScale;
		}
	}

	for (unsigned int r = 0; r < a_dims; ++r)
	{
		for (unsigned int c = 0; c < a_dims; ++c)
		{
			glm::vec3 a = m_vertices[r * a_dims + c].position.xyz;
			glm::vec3 b, d, normal;

			if (r != a_dims - 1 && c != a_dims - 1)
			{
				b = m_vertices[(r + 1) * a_dims + c].position.xyz;
				d = m_vertices[(r + 1) * a_dims + (c + 1)].position.xyz;
				normal = glm::vec3(glm::normalize(glm::cross(b - a, d - a)));
			}
			else if (r == a_dims - 1 && c == a_dims - 1)
			{
				b = m_vertices[(r - 1) * a_dims + c].position.xyz;
				d = m_vertices[(r - 1) * a_dims + (c - 1)].position.xyz;
				normal = glm::vec3(glm::normalize(glm::cross(b - a, d - a)));
			}
			else if (r == a_dims - 1 && c != a_dims - 1)
			{
				b = m_vertices[(r - 1) * a_dims + c].position.xyz;
				d = m_vertices[(r - 1) * a_dims + (c + 1)].position.xyz;
				normal = glm::vec3(glm::normalize(glm::cross(b - a, d - a))*-1.0f);
			}
			else if (r != a_dims - 1 && c == a_dims - 1)
			{
				b = m_vertices[(r + 1) * a_dims + c].position.xyz;
				d = m_vertices[(r + 1) * a_dims + (c - 1)].position.xyz;
				normal = glm::vec3(glm::normalize(glm::cross(b - a, d - a))*-1.0f);
			}

			m_vertices[r * a_dims + c].normal = glm::vec4(normal, 1.0);
			m_vertices[r*a_dims + c].tangent = glm::vec4(glm::cross(normal, glm::vec3(0, 1, 0)), 1.0f);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	m_indices = new unsigned int[((a_dims - 1) * (a_dims - 1) * 6)];
	m_indexCount = 0;

	for (unsigned int r = 0; r < (a_dims - 1); ++r)
	{
		for (unsigned int c = 0; c < (a_dims - 1); ++c)
		{
			// triangle 1
			m_indices[m_indexCount++] = r * a_dims + c;
			m_indices[m_indexCount++] = (r + 1) * a_dims + c;
			m_indices[m_indexCount++] = (r + 1) * a_dims + (c + 1);
			// triangle 2
			m_indices[m_indexCount++] = r * a_dims + c;
			m_indices[m_indexCount++] = (r + 1) * a_dims + (c + 1);
			m_indices[m_indexCount++] = r * a_dims + (c + 1);
		}
	}
}

void SegmentedGrid::ApplyMapToBuffers()
{
	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (m_dims * m_dims) * sizeof(VertexData), m_vertices, GL_STATIC_DRAW);

	// Initialise Vertex Element data
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(VertexData),	(void*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,	sizeof(VertexData),	(void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), m_indices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	
	delete[] m_vertices;
	delete[] m_indices;
}

void SegmentedGrid::Render(unsigned int* a_programID)
{
	glUseProgram(*a_programID);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
}