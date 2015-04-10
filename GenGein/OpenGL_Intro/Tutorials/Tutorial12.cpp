#include <stack>
#include <gl_core_4_4.h>
#include <glm\ext.hpp>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\Renderer\TextureHandler.h"
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

void Tutorial12::SetUpShadowRender()
{
	// Setup shadow map buffer
	glGenFramebuffers(1, &m_shadowID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowID);

	glGenTextures(1, &m_FBODepth);
	glBindTexture(GL_TEXTURE_2D, m_FBODepth);

	// Texture uses a 16-bit depth component format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 4096, 4096,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// attached as a depth attachment to capture depth not colour
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		m_FBODepth, 0);

	// No colour targets are used
	glDrawBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR: FRAMEBUFFER UNSUCCESSFUL.\n");
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tutorial12::CalcLightingMatrix()
{
	m_lightDirection = glm::normalize(glm::vec3(-35, 339, 118));

	glm::mat4 lightProjection = glm::ortho<float>(
		-100, 100, -100, 100, -100, 100);

	glm::mat4 lightView = glm::lookAt(m_lightDirection,
		glm::vec3(0), glm::vec3(0, 1, 0));

	m_lightMatrix = lightProjection * lightView;

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Debug", "Sun Dir", TW_TYPE_DIR3F, (void*)&m_lightDirection[0]);
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
	
	float pscale = (1.0f / a_dim) * 3;
	int octaves = 6;

	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			m_enviroVerts[r * a_dim + c].position = glm::vec4(-center + (float)(c * scale), 0, -center + (float)(r * scale), 1);
			m_enviroVerts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);

			// Perlin data
			float persistence = 0.3f;
			float ampli = m_amplitude;
			for (int o = 0; o < octaves; ++o)	
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin((glm::vec2((float)c, (float)r) + m_seeder) * pscale * freq) * 0.5f + 0.5f;

				m_enviroVerts[r * a_dim + c].position.y += -m_amplitude/10 + (perlin_sample * ampli);
				ampli *= persistence;
			}

			// Save off possible tree data for tree positions
			vec3 scale = vec3(1);
			float angle = 0;
			vec3 direction = vec3(1);
			
			int range = 10;
			uint treeSeed = r * a_dim + c;
			treeSeed += -range / 2 + (rand() % range);
			if (treeSeed == (r * a_dim + c) && m_enviroVerts[r * a_dim + c].position.y > 3)
			{
				m_treeSpawns.push_back(
					glm::scale(scale) *
					glm::rotate(angle, direction) *
					glm::translate(vec3(m_enviroVerts[r * a_dim + c].position)));
			}
		}
	}

	// Calculating normals
	for (GLuint row = 0; row < a_dim; ++row)
	{
		for (GLuint col = 0; col < a_dim; ++col)
		{
			int nextRowInc = 1, nextColInc = 1;

			if (row == a_dim - 1) nextRowInc = -1;
			else nextRowInc = 1;
			if (col == a_dim - 1) nextColInc = -1;
			else nextColInc = 1;

			//current | across | across + above
			uint a = col*a_dim + row;
			uint b = a + (a_dim * nextColInc);
			uint c = a + nextColInc;

			vec3 dir = glm::cross(vec3(m_enviroVerts[b].position - m_enviroVerts[a].position), vec3(m_enviroVerts[c].position - m_enviroVerts[a].position));
			m_enviroVerts[a].normal = vec4(normalize(dir),0);
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
	glBufferData(GL_ARRAY_BUFFER, a_dim * a_dim * sizeof(VertexData), &m_enviroVerts[0], GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

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

	m_amplitude = 100;
	m_seeder = 12;

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(218, 146, 167), glm::vec3(0, 0, 0));
	
	ShaderHandler::LoadShaderProgram("WaterMap",
		"Data/Shaders/ProGenReGen.vert",
		"Data/Shaders/ProGenReGen.frag");
	m_pMainProgramID = &ShaderHandler::GetShader("WaterMap");
	
	ShaderHandler::LoadShaderProgram("EnviroMap",
		"Data/Shaders/Test/Enviro.vert",
		"Data/Shaders/Test/Enviro.frag");
	m_enviroProg = &ShaderHandler::GetShader("EnviroMap");

	ShaderHandler::LoadShaderProgram("GenShadow",
		"Data/Shaders/GenShadow.vert",
		"Data/Shaders/GenShadow.frag");
	m_genShadowProg = &ShaderHandler::GetShader("GenShadow");

	m_lightDirUniLoc = glGetUniformLocation(*m_enviroProg, "lightDir");
	m_shadowMapUniLoc = glGetUniformLocation(*m_enviroProg, "shadowMap");
	m_ulightMatUniLoc = glGetUniformLocation(*m_enviroProg, "LightMatrix");
	m_glightMatUniLoc = glGetUniformLocation(*m_genShadowProg, "LightMatrix");

	//SetUpShadowRender();
	//CalcLightingMatrix();

	// WATER PLANE
	CreatePerlinPlane(2);

	// ENVIRO PLANE
	CreateEnviroGrid(128);

	//TextureHandler::LoadPerlin(m_enviroProg, "heightMap", 128);
	TextureHandler::LoadTexture(m_enviroProg, "SandMap", "Data/Textures/sand_tile.jpg");
	TextureHandler::LoadTexture(m_enviroProg, "GrassMap", "Data/Textures/grass_tiled.tga");

	m_palmTree = new ObjMesh(vec3(0));
	m_palmTree->LoadObject("Data/Objects/Cow", "testCow");

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Seed", TwType::TW_TYPE_FLOAT, (void*)&m_seeder);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Amplitude", TwType::TW_TYPE_FLOAT, (void*)&m_amplitude);
}

// Destroy things
void Tutorial12::ShutDown()
{

}

// Update loop
void Tutorial12::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);

	if (glfwGetKey(m_pWindow, GLFW_KEY_O))
	{
		m_treeSpawns.clear();
		CreateEnviroGrid(128);
	}
}

void Tutorial12::UpdateShadowTexture()
{
	// Final pass: bind back-buffer and clear colour and depth
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowID);
	glViewport(0, 0, 4096, 4096);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_genShadowProg);
	glUniformMatrix4fv(m_glightMatUniLoc, 1, GL_FALSE, &m_lightMatrix[0][0]);

	glUseProgram(*m_enviroProg);
	glBindVertexArray(m_enviroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVAO);
	glDrawElements(GL_TRIANGLES, m_enviroIndexCount, GL_UNSIGNED_INT, 0);
}

// Render things to screen
void Tutorial12::Render()
{
	GLApplication::Render();
	TextureHandler::RenderAllTextures();

	glUseProgram(*m_enviroProg);
	glBindVertexArray(m_enviroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVAO);
	glDrawElements(GL_TRIANGLES, m_enviroIndexCount, GL_UNSIGNED_INT, 0);

	//bind the light matrix
	glm::mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;

	glUniformMatrix4fv(m_ulightMatUniLoc, 1, GL_FALSE, &lightMatrix[0][0]);
	glUniform3fv(m_lightDirUniLoc, 1, &m_lightDirection[0]);
	glUniform1i(m_shadowMapUniLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBODepth);

	for (uint i = 0; i < m_treeSpawns.size(); i++)
	{
		m_palmTree->Render(m_treeSpawns[i]);
	}

	glUseProgram(*m_pMainProgramID);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}