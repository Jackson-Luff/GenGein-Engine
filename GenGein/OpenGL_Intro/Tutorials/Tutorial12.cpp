#include <stack>
#include <gl_core_4_4.h>
#include <glm\ext.hpp>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Textures\TextureHandler.h"
#include "Cameras\BaseCamera.h"

#include "Tutorial12.h"

Tutorial12::Tutorial12()
{
}

// Construct with specified window attributes
Tutorial12::Tutorial12(const int a_width, const int a_height, const char* a_title)
	: GLApplication(a_width, a_height, a_title)
{}

Tutorial12::~Tutorial12()
{
}

void Tutorial12::CreatePerlinPlane(c_uint a_dim)
{
	m_dims = a_dim;

	float scale = ((a_dim) / 2) * 500.0f;
	// Create a center-point based on input
	float center = (scale * a_dim) / 1.5f;

	// Populate verts with row + cols input
	VertexData* verts = new VertexData[a_dim * a_dim];

	for (uint r = 0; r <a_dim; ++r)
	{
		for (uint c = 0; c < a_dim; ++c)
		{
			verts[r * a_dim + c].position = glm::vec4(-center + (c * scale), 0, -center + (r * scale), 0);
			verts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* indices = new GLuint[((a_dim - 1) * (a_dim - 1) * 6)];
	m_indexCount = 0;

	for (uint r = 0; r < (a_dim - 1); ++r)
	{
		for (uint c = 0; c < (a_dim - 1); ++c)
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

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	uint vbo, ibo;
	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, a_dim * a_dim * sizeof(VertexData), verts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(uint), indices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] verts;
	delete[] indices;

	CreateTexture(128);
}

void Tutorial12::CreateTexture(c_uint a_dim)
{
	float* perlin_data = new float[a_dim * a_dim];
	float scale = (1.0f / a_dim) * 3;
	int octaves = 6;
	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlin_data[y * a_dim + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample =
					glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * a_dim + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	perlinTextureUniLoc = glGetUniformLocation(*m_enviroProg, "heightMap");

	delete [] perlin_data;
}

void Tutorial12::CreateEnviroGrid(c_uint a_dim)
{
	// Create a center-point based on input
	glm::vec2 center = glm::vec2(a_dim / 2.0f, a_dim / 2.0f);
	float scale = ((a_dim + a_dim) / 2) / 25.0f;

	// Populate verts with row + cols input

	VertexData* m_enviroVerts = new VertexData[a_dim * a_dim];

	for (GLuint r = 0; r <a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			m_enviroVerts[r * a_dim + c].position = glm::vec4((-150.0f + c) * scale, 0, (-150.0f + r) * scale, 1);
			m_enviroVerts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* auiIndices = new GLuint[((a_dim - 1) * (a_dim - 1) * 6)];
	m_enviroIndexCount = 0;

	for (GLuint r = 0; r < (a_dim - 1); ++r)
	{
		for (GLuint c = 0; c < (a_dim - 1); ++c)
		{
			// triangle 1
			auiIndices[m_enviroIndexCount++] = r * a_dim + c;
			auiIndices[m_enviroIndexCount++] = (r + 1) * a_dim + c;
			auiIndices[m_enviroIndexCount++] = (r + 1) * a_dim + (c + 1);
			// triangle 2
			auiIndices[m_enviroIndexCount++] = r * a_dim + c;
			auiIndices[m_enviroIndexCount++] = (r + 1) * a_dim + (c + 1);
			auiIndices[m_enviroIndexCount++] = r * a_dim + (c + 1);
		}
	}

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_enviroVAO);
	glBindVertexArray(m_enviroVAO);

	uint vbo,ibo;
	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, a_dim * a_dim * sizeof(VertexData), m_enviroVerts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_enviroIndexCount * sizeof(uint), auiIndices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] auiIndices;
}

// Initialise Member Variables
void Tutorial12::StartUp()
{
	GLApplication::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(-571, 0.5, 342), glm::vec3(-426, 0, -374));
	
	ShaderHandler::LoadShaderProgram("PerlinNoise",
		"Data/Shaders/ProGenReGen.vert",
		"Data/Shaders/ProGenReGen.frag");
	m_pMainProgramID = &ShaderHandler::GetShader("PerlinNoise");

	ShaderHandler::LoadShaderProgram("EnviroMap",
		"Data/Shaders/Test/Enviro.vert",
		"Data/Shaders/Test/Enviro.frag");
	m_enviroProg = &ShaderHandler::GetShader("EnviroMap");

	CreatePerlinPlane(2);
	CreateEnviroGrid(128);
}

// Destroy things
void Tutorial12::ShutDown()
{

}

// Update loop
void Tutorial12::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

// Render things to screen
void Tutorial12::Render()
{
	GLApplication::Render();
	
	TextureHandler::RenderAllTextures();

	//glUseProgram(*m_pMainProgramID);
	//glBindVertexArray(m_VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	//glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	
	glUseProgram(*m_enviroProg);
	glBindVertexArray(m_enviroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVAO);
	glDrawElements(GL_TRIANGLES, m_enviroIndexCount, GL_UNSIGNED_INT, 0);
}