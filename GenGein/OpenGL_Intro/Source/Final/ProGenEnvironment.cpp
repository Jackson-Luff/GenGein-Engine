#include <stack>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Particles\GPUParticleEmitter.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Cameras\BaseCamera.h"

#include "ProGenEnvironment.h"

ProGenEnvironment::ProGenEnvironment()
{
}

// Construct with specified window attributes
ProGenEnvironment::ProGenEnvironment(const int a_width, const int a_height, const char* a_title)
	: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
ProGenEnvironment::~ProGenEnvironment()
{
}

// Creates segmented plane used as the water plane. 
void ProGenEnvironment::CreateWaterGrid(c_uint a_dim)
{
	float scale = ((a_dim) / 2) * 0.15f;
	// Create a center-point based on input
	float center = (a_dim*scale) * 0.5f;

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
	m_waterIndexCount = 0;

	for (GLuint r = 0; r < (a_dim - 1); ++r)
	{
		for (GLuint c = 0; c < (a_dim - 1); ++c)
		{
			// triangle 1
			indices[m_waterIndexCount++] = r * a_dim + c;
			indices[m_waterIndexCount++] = (r + 1) * a_dim + c;
			indices[m_waterIndexCount++] = (r + 1) * a_dim + (c + 1);
			// triangle 2
			indices[m_waterIndexCount++] = r * a_dim + c;
			indices[m_waterIndexCount++] = (r + 1) * a_dim + (c + 1);
			indices[m_waterIndexCount++] = r * a_dim + (c + 1);
		}
	}

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_waterVAO);
	glBindVertexArray(m_waterVAO);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_waterIndexCount * sizeof(uint), indices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] verts;
	delete[] indices;
}

// Creates segmented plane used as the land plane. 
void ProGenEnvironment::CreateEnviroGrid(c_uint a_dim)
{
	// Create a center-point based on input
	float scale = ((a_dim + a_dim) / 2) / 40.0f;
	float center = (a_dim*scale) * 0.5f;

	// Populate verts with row + cols input
	VertexData* m_enviroVerts = new VertexData[a_dim * a_dim];

	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			m_enviroVerts[r * a_dim + c].position = glm::vec4(-center + (float)(c * scale), 0, -center + (float)(r * scale), 1);
			m_enviroVerts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);

			// Perlin data
			float pscale = (1.0f / a_dim) * 3;
			float persistence = 0.3f;
			float ampli = m_amplitude;
			int octaves = 6;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin((glm::vec2((float)r, (float)c) + m_seeder) * pscale * freq) + 0.5f;

				m_enviroVerts[r * a_dim + c].position.y += -m_amplitude / 10 + (perlin_sample * ampli * 2);
				ampli *= persistence;
			}
		}
	}

	// Calculating normals and spawns
	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			vec3 a = m_enviroVerts[r * a_dim + c].position.xyz;
			vec3 b;
			vec3 d;
			if (r != a_dim - 1 && c != a_dim - 1)
			{
				b = m_enviroVerts[(r + 1) * a_dim + c].position.xyz;
				d = m_enviroVerts[(r + 1) * a_dim + (c + 1)].position.xyz;
				m_enviroVerts[r * a_dim + c].normal = vec4(glm::normalize(glm::cross(b - a, d - a)), 1);
			}

			// Save off possible tree data for tree positions
			uint treeSeed = r * a_dim + c;
			treeSeed += (-m_range / 2.0f) + (rand() % (int)m_range);
			if (treeSeed == (r * a_dim + c) && a.y > 3)
			{
				m_treeSpawns.push_back(
					glm::translate(vec3(a)) *
					glm::rotate(dot(vec3(m_enviroVerts[r * a_dim + c].normal), vec3(0, 1, 0)), vec3(m_enviroVerts[r * a_dim + c].normal)));

				m_indexTypeToSpawn.push_back(rand() % 9);
			}
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

// Loads in the tree's that are randomly sampled 
void LoadTrees(std::vector< FBXModel>& a_trees, int a_offset)
{
	for (int i = a_offset; i < a_offset+3; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			FBXModel tree = FBXModel();
			std::string directory = std::string("Data/Models/Ryan/tree");
			directory += std::to_string(i);
			directory += "_0";
			directory += std::to_string(j);
			directory += ".fbx";
			tree.LoadFBX(directory, FBXFile::UNITS_CENTIMETER);

			a_trees.push_back(tree);
		}
	}
}

// Initialise Member Variables
void ProGenEnvironment::StartUp()
{
	GLApplication::StartUp();

	m_range = 100;
	m_amplitude = 50;
	m_seeder = 12;
	m_ambientIntensity = 0.1f;
	m_diffuseIntensity = 1.0f;
	m_specIntensity = 1.0f;
	m_speedOfTime = 0.05f;

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(218, 146, 167), glm::vec3(0, 0, 0));

	ShaderHandler::LoadShaderProgram("WaterMap",
		"Data/Shaders/Used/Water.vert",
		"Data/Shaders/Used/Water.frag");
	m_waterProg = &ShaderHandler::GetShader("WaterMap");
	
	ShaderHandler::LoadShaderProgram("EnviroMap",
		"Data/Shaders/Used/Enviro.vert",
		"Data/Shaders/Used/Enviro.frag");
	m_enviroProg = &ShaderHandler::GetShader("EnviroMap");

	ShaderHandler::LoadShaderProgram("CamWeap",
		"Data/Shaders/Used/CamWeap.vert",
		"Data/Shaders/Used/CamWeap.frag");
	m_camWeapProg = &ShaderHandler::GetShader("CamWeap");

	ShaderHandler::LoadShaderProgram("SunPlane",
		"Data/Shaders/Used/Sun.vert",
		"Data/Shaders/Used/Sun.frag");
	m_sunPlaneProg = &ShaderHandler::GetShader("SunPlane");

	//SetUpShadowRender();
	//CalcLightingMatrix();

	// WATER PLANE
	CreateWaterGrid(128);

	// ENVIRO PLANE
	CreateEnviroGrid(192);

	TextureHandler::LoadPerlin(m_enviroProg, "heightMap", 128);
	//TextureHandler::LoadTexture(m_enviroProg, "Shit", "Data/Textures/Environment/grass_diff.png");
	TextureHandler::LoadTexture(m_enviroProg, "SandMap", "Data/Textures/sand_tile.jpg");
	TextureHandler::LoadTexture(m_enviroProg, "GrassMap", "Data/Textures/grass_tiled.tga");
	TextureHandler::LoadTexture(m_enviroProg, "StoneMap", "Data/Textures/dirt_tiled.tga");

	m_particleEmitter = new GPUParticleEmitter();
	m_particleEmitter->Initialise(
		1000000,
		1.0f, 28.0f,
		1.0f, 4.1f,
		0.01f, 0.7f,
		vec4(0, 0, 1, 1),
		vec4(0.5, 0, 1, 1));

	m_sunModel = FBXModel();
	m_sunModel.LoadFBX(m_sunPlaneProg,
		"Data/Models/plane.fbx",
		FBXFile::UNITS_CENTIMETER);
	m_sunModel.SetPosition(vec3(0, 300, 0));

	m_camWeapModel = FBXModel();
	m_camWeapModel.LoadFBX(
		m_camWeapProg,
		"Data/Models/GUNs/GUNFBX.fbx",
		FBXFile::UNITS_CENTIMETER);
	m_camWeapMatrix = glm::mat4(1);

	LoadTrees(m_pineTreeModels, 1);
	LoadTrees(m_palmTreeModels, 4);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Seed"		, TwType::TW_TYPE_FLOAT, (void*)&m_seeder);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Amplitude"	, TwType::TW_TYPE_FLOAT, (void*)&m_amplitude);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Population", TwType::TW_TYPE_FLOAT, (void*)&m_range);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Time Speed", TwType::TW_TYPE_FLOAT, (void*)&m_speedOfTime);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Lighting", "Ambient  %", TwType::TW_TYPE_FLOAT, (void*)&m_ambientIntensity);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Lighting", "Diffuse  %", TwType::TW_TYPE_FLOAT, (void*)&m_diffuseIntensity);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Lighting", "Specular %", TwType::TW_TYPE_FLOAT, (void*)&m_specIntensity);

}

// Destroy things
void ProGenEnvironment::ShutDown()
{
}

// Update loop
void ProGenEnvironment::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);

	if (glfwGetKey(m_pWindow, GLFW_KEY_O))
	{
		m_treeSpawns.clear();
		CreateEnviroGrid(192);
	}
}

// Render things to screen
void ProGenEnvironment::Render()
{
	GLApplication::Render();

	ApplyLightingSetup(vec3(m_ambientIntensity),
		m_sunPosition,
		m_startLightingHeight);

	m_camWeapMatrix = glm::translate(vec3(2,-1,-3)) *
		glm::rotate(90.0f, vec3(1,-1, 1)) *
		glm::scale(vec3(0.05f));

	m_camWeapModel.SetLocalTransform(m_pBaseCamera->GetWorldTransform() * m_camWeapMatrix);
	m_camWeapModel.Render();

	m_particleEmitter->SetPosition(vec3( (m_pBaseCamera->GetWorldTransform() * glm::translate(vec3(9, -7,-25))[3] ) ) );
	m_particleEmitter->Render(
		(float)GetDeltaTime(),
		(float)GetElapsedTime(),
		(m_pBaseCamera->GetWorldTransform()),
		(float)glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1));

	m_sunPosition = vec3(0.0f, 
		500.0f * sin(GetElapsedTime()*m_speedOfTime),
		500.0f * cos(GetElapsedTime()*m_speedOfTime));
	
	if (glfwGetKey(m_pWindow, GLFW_KEY_T)) { m_sunPosition = vec3(0.0f, 500.0f, 500.0f);}
	
	m_sunModel.SetPosition(m_sunPosition);
	m_sunModel.Render();

	TextureHandler::RenderAllTextures();

	glUseProgram(*m_enviroProg);
	glBindVertexArray(m_enviroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_enviroVAO);
	glDrawElements(GL_TRIANGLES, m_enviroIndexCount, GL_UNSIGNED_INT, 0);
	
	glEnable(GL_BLEND);
	
	glUseProgram(*m_waterProg);
	glBindVertexArray(m_waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_waterVAO);
	glDrawElements(GL_TRIANGLES, m_waterIndexCount, GL_UNSIGNED_INT, 0);

	for (uint i = 0; i < m_treeSpawns.size(); i++)
	{
		if (m_treeSpawns[i][3].y > 13)
		{
			m_pineTreeModels[m_indexTypeToSpawn[i]].SetLocalTransform(m_treeSpawns[i] * glm::scale(vec3(0.025f)));
			m_pineTreeModels[m_indexTypeToSpawn[i]].Render();
		}
		else
		{
			m_palmTreeModels[m_indexTypeToSpawn[i]].SetLocalTransform(m_treeSpawns[i] * glm::scale(vec3(0.1f)));
			m_palmTreeModels[m_indexTypeToSpawn[i]].Render();
		}
	}
	glDisable(GL_BLEND);
}