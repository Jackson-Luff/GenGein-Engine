#include <stack>
#include <gl_core_4_4.h>
#include <glm\ext.hpp>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Textures\TextureHandler.h"
#include "Engine\Objects\ObjMesh.h"
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
	float center = (a_dim * scale) * 0.25f;

	// Populate verts with row + cols input
	VertexData* verts = new VertexData[a_dim * a_dim];

	for (GLuint r = 0; r <a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			verts[r * a_dim + c].position = glm::vec4(-center + (float)c * scale, 0, -center + (float)r * scale, 0);
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
}

void Tutorial12::CreateEnviroGrid(c_uint a_dim)
{
	// Create a center-point based on input
	float scale = ((a_dim + a_dim) / 2) / 50.0f;
	float center = (a_dim*scale) * 0.5f;

	// Populate verts with row + cols input

	VertexData* m_enviroVerts = new VertexData[a_dim * a_dim];

	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			m_enviroVerts[r * a_dim + c].position = glm::vec4(-center + (float)(c * scale), 0, -center + (float)(r * scale), 1);
			m_enviroVerts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* indexData = new GLuint[((a_dim - 1) * (a_dim - 1) * 6)];
	m_enviroIndexCount = 0;

	for (GLuint r = 0; r < (a_dim - 1); ++r)
	{
		for (GLuint c = 0; c < (a_dim - 1); ++c)
		{
			// triangle 1
			indexData[m_enviroIndexCount++] = r * a_dim + c;
			indexData[m_enviroIndexCount++] = (r + 1) * a_dim + c;
			indexData[m_enviroIndexCount++] = (r + 1) * a_dim + (c + 1);
			// triangle 2
			indexData[m_enviroIndexCount++] = r * a_dim + c;
			indexData[m_enviroIndexCount++] = (r + 1) * a_dim + (c + 1);
			indexData[m_enviroIndexCount++] = r * a_dim + (c + 1);
		}
	}

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_enviroVAO);
	glBindVertexArray(m_enviroVAO);

	uint vbo, ibo;
	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, a_dim * a_dim * sizeof(VertexData), m_enviroVerts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_enviroIndexCount * sizeof(uint), indexData, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] indexData;
}

// Initialise Member Variables
void Tutorial12::StartUp()
{
	GLApplication::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(0, 0.5, 0), glm::vec3(50, 0, 50));
	
	ShaderHandler::LoadShaderProgram("WaterMap",
		"Data/Shaders/ProGenReGen.vert",
		"Data/Shaders/ProGenReGen.frag");
	m_pMainProgramID = &ShaderHandler::GetShader("WaterMap");
	
	ShaderHandler::LoadShaderProgram("EnviroMap",
		"Data/Shaders/Test/Enviro.vert",
		"Data/Shaders/Test/Enviro.frag");
	m_enviroProg = &ShaderHandler::GetShader("EnviroMap");
	
	// WATER PLANE
	CreatePerlinPlane(2);

	// ENVIRO PLANE
	CreateEnviroGrid(128);

	TextureHandler::LoadPerlin(*m_enviroProg, "heightMap", 128);
	TextureHandler::LoadTexture(*m_enviroProg, "SandMap", "Data/Textures/sand_tile.jpg");
	TextureHandler::LoadTexture(*m_enviroProg, "GrassMap", "Data/Textures/grass_tiled.tga");

	m_palmTree = new ObjMesh(vec3(0));
	m_palmTree->LoadObject("Data/Objects/PalmTree02", "Palma 001");
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

	m_palmTree->Render();

	glUseProgram(*m_enviroProg);
	glBindVertexArray(m_enviroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVAO);
	glDrawElements(GL_TRIANGLES, m_enviroIndexCount, GL_UNSIGNED_INT, 0);

	glUseProgram(*m_pMainProgramID);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}