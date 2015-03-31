#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Textures\TextureHandler.h"
#include "Engine\Renderer\FBOMesh.h"
#include "Engine\Lighting.h"

#include "Cameras\FlyCamera.h"
#include "Tutorial10.h"

using glm::vec3;
using glm::vec4;

// Default constructor
Tutorial10::Tutorial10()
{}

// Construct with specified window attributes
Tutorial10::Tutorial10(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial10::~Tutorial10()
{}

void Tutorial10::CreateWaterGrid(c_uint a_rows, c_uint a_cols)
{
	// Create a center-point based on input
	glm::vec2 center = glm::vec2(a_rows / 2.0f, a_cols / 2.0f);
	float scale = ((a_rows + a_cols) / 2) / 25.0f;
	m_waterRowCount = a_rows;
	m_waterColCount = a_cols;

	// Populate verts with row + cols input

	m_waterVerts = new VertexData[a_rows * a_cols];
	
	for (GLuint r = 0; r <a_rows; ++r)
	{
		for (GLuint c = 0; c < a_cols; ++c)
		{
			m_waterVerts[r * a_cols + c].position = glm::vec3(-center.x + (float)c * scale, 0, -center.y + (float)r * scale);
			m_waterVerts[r * a_cols + c].normal = glm::vec3(0, 1, 0);
			m_waterVerts[r * a_cols + c].uv = glm::vec2((float)r / a_rows, (float)c / a_rows);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* auiIndices = new GLuint[((a_rows - 1) * (a_cols - 1) * 6)];
	m_waterIndexCount = 0;

	for (GLuint r = 0; r < (a_rows - 1); ++r)
	{
		for (GLuint c = 0; c < (a_cols - 1); ++c)
		{
			// triangle 1
			auiIndices[m_waterIndexCount++] = r * a_rows + c;
			auiIndices[m_waterIndexCount++] = (r + 1) * a_rows + c;
			auiIndices[m_waterIndexCount++] = (r + 1) * a_rows + (c + 1);
			// triangle 2
			auiIndices[m_waterIndexCount++] = r * a_rows + c;
			auiIndices[m_waterIndexCount++] = (r + 1) * a_rows + (c + 1);
			auiIndices[m_waterIndexCount++] = r * a_rows + (c + 1);
		}
	}

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_waterVAO);
	glBindVertexArray(m_waterVAO);

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_waterVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_waterVBO);
	glBufferData(GL_ARRAY_BUFFER, a_rows * a_cols * sizeof(VertexData), m_waterVerts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_waterIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_waterIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_waterIndexCount * sizeof(uint), auiIndices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] auiIndices;
}

void Tutorial10::CreateEnviroGrid(c_uint a_rows, c_uint a_cols)
{
	// Create a center-point based on input
	glm::vec2 center = glm::vec2(a_rows / 2.0f, a_cols / 2.0f);
	float scale = ((a_rows + a_cols) / 2) / 25.0f;
	m_enviroRowCount = a_rows;
	m_enviroColCount = a_cols;

	// Populate verts with row + cols input

	m_enviroVerts = new VertexData[a_rows * a_cols];

	for (GLuint r = 0; r <a_rows; ++r)
	{
		for (GLuint c = 0; c < a_cols; ++c)
		{
			m_enviroVerts[r * a_cols + c].position = glm::vec3(-center.x + (float)c * scale, 0, -center.y + (float)r * scale);
			m_enviroVerts[r * a_cols + c].normal = glm::vec3(0, 1, 0);
			m_enviroVerts[r * a_cols + c].uv = glm::vec2((float)r / a_rows, (float)c / a_rows);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* auiIndices = new GLuint[((a_rows - 1) * (a_cols - 1) * 6)];
	m_enviroIndexCount = 0;

	for (GLuint r = 0; r < (a_rows - 1); ++r)
	{
		for (GLuint c = 0; c < (a_cols - 1); ++c)
		{
			// triangle 1
			auiIndices[m_enviroIndexCount++] = r * a_rows + c;
			auiIndices[m_enviroIndexCount++] = (r + 1) * a_rows + c;
			auiIndices[m_enviroIndexCount++] = (r + 1) * a_rows + (c + 1);
			// triangle 2
			auiIndices[m_enviroIndexCount++] = r * a_rows + c;
			auiIndices[m_enviroIndexCount++] = (r + 1) * a_rows + (c + 1);
			auiIndices[m_enviroIndexCount++] = r * a_rows + (c + 1);
		}
	}

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_enviroVAO);
	glBindVertexArray(m_enviroVAO);

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_enviroVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVBO);
	glBufferData(GL_ARRAY_BUFFER, a_rows * a_cols * sizeof(VertexData), m_enviroVerts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_enviroIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_enviroIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_enviroIndexCount * sizeof(uint), auiIndices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] auiIndices;
}

// Initialise things
void Tutorial10::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();

	ShaderHandler::LoadShaderProgram("WaterProgram",
		"Data/Shaders/Test/Water.vert",
		"Data/Shaders/Test/Water.frag");
	m_waterProg = &ShaderHandler::GetShader("WaterProgram");
	 
	ShaderHandler::LoadShaderProgram("EnviroProgram",
		"Data/Shaders/Test/Enviro.vert",
		"Data/Shaders/Test/Enviro.frag");
	m_enviroProg = &ShaderHandler::GetShader("EnviroProgram");

	//Initialise camera
	InitialiseFlyCamera(5.f, 20.0f, 0.1f,
		glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

	m_waterRender = new FBOMesh();
	m_waterRender->Initialise(128, 128);

	CreateWaterGrid(128, 128);
	CreateEnviroGrid(128, 128);

	// Water Texture use

	TextureHandler::LoadTexture(*m_waterProg, "heightMap",
		"Data/Textures/water.jpg");

	TextureHandler::LoadTexture(*m_waterProg, "water",
		"Data/Textures/water01.jpg");

	// Enviro Texture use

	TextureHandler::LoadTexture(*m_enviroProg, "envheightMap",
		"Data/Textures/PerlinNoise/Noise2.jpg");

	TextureHandler::LoadTexture(*m_enviroProg, "grassMap",
		"Data/Textures/grass.png");

	TextureHandler::LoadTexture(*m_enviroProg, "stoneMap",
		"Data/Textures/stone.jpg");

	TextureHandler::LoadTexture(*m_enviroProg, "grasssMap",
		"Data/Textures/GRASSS.png");
}

// Destroy things 
void Tutorial10::ShutDown()
{
}

// Updates inputs and incr time
void Tutorial10::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

void Tutorial10::RenderWaterPlane()
{
	glUseProgram(*m_waterProg);
	// Create and bind buffers to a vertex array object
	for (uint row = 0; row < m_waterRowCount; ++row)
	{
		for (uint col = 0; col < m_waterColCount; ++col)
		{
			int nextRowInc = 1, nextColInc = 1;

			if (row == m_waterRowCount - 1) nextRowInc = -1;
			else nextRowInc = 1;
			if (col == m_waterColCount - 1) nextColInc = -1;
			else nextColInc = 1;

			//current | across | across + above
			uint a = col*m_waterColCount + row;
			uint b = a + (m_waterColCount * nextColInc);
			uint c = a + nextColInc;

			vec3 dir = glm::cross(m_waterVerts[b].position - m_waterVerts[a].position, m_waterVerts[c].position - m_waterVerts[a].position);
			m_waterVerts[a].normal = normalize(dir);
			m_waterVerts[a].uv.x += (float)GetDeltaTime() * 0.01f;
		}
	}

	glBindVertexArray(m_waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_waterVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_waterRowCount * m_waterColCount * sizeof(VertexData), m_waterVerts);
	glDrawElements(GL_TRIANGLES, m_waterIndexCount, GL_UNSIGNED_INT, 0);
}

void Tutorial10::RenderEnviroPlane()
{
	glUseProgram(*m_enviroProg);
	glBindVertexArray(m_enviroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_enviroRowCount * m_enviroColCount * sizeof(VertexData), m_enviroVerts);
	glDrawElements(GL_TRIANGLES, m_enviroIndexCount, GL_UNSIGNED_INT, 0);
}

// Draws the stuff to screen
void Tutorial10::Render()
{
	GLApplication::Render();
	
	TextureHandler::RenderAllTextures();

	RenderWaterPlane();

	RenderEnviroPlane();
}