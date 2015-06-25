#include <stack>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Particles\GPUParticleEmitter.h"
#include "Engine\Renderer\SkyBox.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\Cameras\BaseCamera.h"

#include "ProGenEnvironment.h"

ProGenEnvironment::ProGenEnvironment()
{
}

// Construct with specified window attributes
ProGenEnvironment::ProGenEnvironment(const int a_width, const int a_height, const char* a_title)
: BaseApp(a_width, a_height, a_title)
{}

// Deconstructor
ProGenEnvironment::~ProGenEnvironment()
{
	delete m_particleEmitter;
	delete m_sunModel;
	delete m_camWeapModel;

	for (auto it : m_palmTreeModels)
	{
		delete it;
	}
	m_palmTreeModels.clear();

	for (auto it : m_pineTreeModels)
	{
		delete it;
	}
	m_pineTreeModels.clear();
}

// Creates segmented plane used as the water plane. 
void ProGenEnvironment::CreateWaterGrid(c_uint a_dim)
{
	float scale = ((a_dim) / 2) * 0.55f;
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
	m_wIndexCount = 0;

	for (GLuint r = 0; r < (a_dim - 1); ++r)
	{
		for (GLuint c = 0; c < (a_dim - 1); ++c)
		{
			// triangle 1
			indices[m_wIndexCount++] = r * a_dim + c;
			indices[m_wIndexCount++] = (r + 1) * a_dim + c;
			indices[m_wIndexCount++] = (r + 1) * a_dim + (c + 1);
			// triangle 2
			indices[m_wIndexCount++] = r * a_dim + c;
			indices[m_wIndexCount++] = (r + 1) * a_dim + (c + 1);
			indices[m_wIndexCount++] = r * a_dim + (c + 1);
		}
	}

	//m_reflectionTexture = FBOMesh();
	//m_reflectionTexture.Initialise(m_waterProg,GetWidth(), GetHeight(), verts, indices);

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_wVAO);
	glBindVertexArray(m_wVAO);

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_wVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_wVBO);
	glBufferData(GL_ARRAY_BUFFER, a_dim * a_dim * sizeof(VertexData), verts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_wIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_wIndexCount * sizeof(uint), indices, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] verts;
	delete[] indices;
}

// Creates segmented plane used as the land plane. 
void ProGenEnvironment::CreateEnviroGrid(c_uint a_dim)
{
	struct EnviroVertexData
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 uv;
	};

	// Create a center-point based on input
	float scale = ((a_dim + a_dim) / 2) / 10.0f;
	float center = (a_dim*scale) * 0.5f;

	// Populate verts with row + cols input
	EnviroVertexData* verts = new EnviroVertexData[a_dim * a_dim];

	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			verts[r * a_dim + c].position = glm::vec4(-center + (float)(c * scale), 0, -center + (float)(r * scale), 1);
			verts[r * a_dim + c].uv = glm::vec2((float)r / a_dim, (float)c / a_dim);

			// Perlin data
			float pscale = (1.0f / a_dim) * 3;
			float persistence = m_persistence;
			float ampli = m_amplitude;
			float octaves = m_octaves;

			for (float o = 0; o < octaves; ++o)
			{
				float freq = powf(m_freq, (float)o);
				float perlin_sample = glm::perlin((glm::vec2((float)r, (float)c) + m_seeder) * pscale * freq) + 0.5f;

				verts[r * a_dim + c].position.y += -m_amplitude / 10 + (perlin_sample * ampli * 2);
				ampli *= persistence;
			}

			if (verts[r * a_dim + c].position.y > m_heightestVertPoint)
				m_heightestVertPoint = verts[r * a_dim + c].position.y;
		}
	}

	// Calculating normals and possible tree spawns
	for (GLuint r = 0; r < a_dim; ++r)
	{
		for (GLuint c = 0; c < a_dim; ++c)
		{
			vec3 a = vec3(verts[r * a_dim + c].position);
			vec3 b;
			vec3 d;
			if (r != a_dim - 1 && c != a_dim - 1)
			{
				b = vec3(verts[(r + 1) * a_dim + c].position);
				d = vec3(verts[(r + 1) * a_dim + (c + 1)].position);
				verts[r * a_dim + c].normal = vec4(glm::normalize(glm::cross(b - a, d - a)), 1);
			}

			//if (glm::dot(vec3(m_eVerts[r * a_dim + c].normal), vec3(0, 1, 0)) < 0.7)
			//{
			//	m_eVerts[r * a_dim + c].position.y = 0.0f;
			//
			//	// calc diamond square (cliff)
			//	CalcDiamondSquare(m_eVerts[r * a_dim + c].position, a_dim);
			//}

			// Save off possible tree data for tree positions
			//int treeSeed = r * a_dim + c;
			//treeSeed += (-m_range / 2) + (rand() % (int)m_range);
			//if (treeSeed == (r * a_dim + c) && a.y > 3)
			//{
			//	m_treeSpawns.push_back(
			//		glm::translate(vec3(a)) *
			//		glm::rotate(dot(vec3(m_eVerts[r * a_dim + c].normal), vec3(0, 1, 0)), vec3(m_eVerts[r * a_dim + c].normal)));
			//
			//	m_indexTypeToSpawn.push_back(rand() % 9);
			//}
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	GLuint* indexData = new GLuint[((a_dim - 1) * (a_dim - 1) * 6)];
	m_eIndexCount = 0;

	for (GLuint r = 0; r < (a_dim - 1); ++r)
	{
		for (GLuint c = 0; c < (a_dim - 1); ++c)
		{
			// triangle 1
			indexData[m_eIndexCount++] = r * a_dim + c;
			indexData[m_eIndexCount++] = (r + 1) * a_dim + c;
			indexData[m_eIndexCount++] = (r + 1) * a_dim + (c + 1);
			// triangle 2
			indexData[m_eIndexCount++] = r * a_dim + c;
			indexData[m_eIndexCount++] = (r + 1) * a_dim + (c + 1);
			indexData[m_eIndexCount++] = r * a_dim + (c + 1);
		}
	}

	//Gen and bind Vertex Array Object
	glGenVertexArrays(1, &m_eVAO);
	glBindVertexArray(m_eVAO);

	// Create and bind buffers to a vertex array object
	glGenBuffers(1, &m_eVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_eVBO);
	glBufferData(GL_ARRAY_BUFFER, a_dim * a_dim * sizeof(EnviroVertexData), verts, GL_DYNAMIC_DRAW);

	// Initialise Vertex Element data 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(EnviroVertexData), (void*)offsetof(EnviroVertexData, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(EnviroVertexData), (void*)offsetof(EnviroVertexData, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EnviroVertexData), (void*)offsetof(EnviroVertexData, uv));

	// Gen and bind data to Index buffer object
	glGenBuffers(1, &m_eIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_eIndexCount * sizeof(uint), indexData, GL_STATIC_DRAW);

	// Unbind VAO (memsave)
	glBindVertexArray(0);
	// Remove pointers (memsave)
	delete[] verts;
	delete[] indexData;
}

void ProGenEnvironment::CalcDiamondSquare(glm::vec4 a_currPos, uint a_dim)
{}

// Loads in the tree's that are randomly sampled 
void LoadTrees(std::vector< FBXModel*>& a_trees, int a_offset)
{
	for (int i = a_offset; i < a_offset + 3; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			FBXModel* tree = new FBXModel();
			std::string directory = std::string("Data/Models/Ryan/tree");
			directory += std::to_string(i);
			directory += "_0";
			directory += std::to_string(j);
			directory += ".fbx";
			tree->LoadFBX(directory, FBXFile::UNITS_CENTIMETER);

			a_trees.push_back(tree);
		}
	}
}

// Initialise Member Variables
void ProGenEnvironment::StartUp()
{
	BaseApp::StartUp();

	m_range = 100;
	m_amplitude = 50;
	m_seeder = 12;
	m_octaves = 6;
	m_ambientIntensity = 0.1f;
	m_diffuseIntensity = 1.0f;
	m_specIntensity = 1.0f;
	m_speedOfTime = 0.05f;
	m_persistence = 0.3f;
	m_freq = 0.3f;
	m_heightestVertPoint = 0.0f;

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(0), glm::vec3(0, 0, 1));

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
	
	ShaderHandler::LoadShaderProgram("Sun",
		"Data/Shaders/Used/Sun.vert",
		"Data/Shaders/Used/Sun.frag");
	
	// WATER PLANE
	CreateWaterGrid(128);

	// ENVIRO PLANE
	CreateEnviroGrid(192);
	
	m_particleEmitter = new GPUParticleEmitter();
	m_particleEmitter->Initialise(
		1000000,
		1.0f, 28.0f,
		1.0f, 50.1f,
		0.1f, 1.7f,
		vec4(1, 0, 0, 1),
		vec4(1, 0.5, 0, 1));

	m_sunModel = new FBXModel();
	m_sunModel->LoadFBX("Sun",
		"Data/Models/sphere.fbx",
		FBXFile::UNITS_CENTIMETER);
	m_sunModel->SetPosition(vec3(0, 0, 0));
	
	//m_FrameBuffObj = TextureHandler::LoadFrameBuffer("Sun", "FBOTexture", GetWidth(), GetHeight());

	m_camWeapModel = new FBXModel();
	m_camWeapModel->LoadFBX(
		"CamWeap",
		"Data/Models/GUNs/GUNFBX.fbx",
		FBXFile::UNITS_CENTIMETER);
	m_camWeapMatrix = glm::mat4(1);
	
	LoadTrees(m_pineTreeModels, 1);
	LoadTrees(m_palmTreeModels, 4);
	
	TextureHandler::LoadTexture("EnviroMap", "SandMap", "Data/Textures/Environment/sand_tile.jpg");
	TextureHandler::LoadTexture("EnviroMap", "GrassMap", "Data/Textures/Environment/shortGrass_diffuse01.png");
	TextureHandler::LoadTexture("EnviroMap", "GravelMap", "Data/Textures/Environment/gravel_diffuse02.png");
	TextureHandler::LoadTexture("EnviroMap", "AsphaltMap", "Data/Textures/Environment/asphalt_diffuse02.png");
	TextureHandler::LoadTexture("EnviroMap", "RockMap", "Data/Textures/Environment/rock_diffuse.png");
	TextureHandler::LoadTexture("EnviroMap", "SnowMap", "Data/Textures/Environment/snow.jpg");
	
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Seed", TwType::TW_TYPE_FLOAT,			(void*)&m_seeder);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Amplitude", TwType::TW_TYPE_FLOAT,		(void*)&m_amplitude);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Population", TwType::TW_TYPE_FLOAT,	(void*)&m_range);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Persistence", TwType::TW_TYPE_FLOAT,	(void*)&m_persistence);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Octaves", TwType::TW_TYPE_FLOAT,		(void*)&m_octaves);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Frequency", TwType::TW_TYPE_FLOAT,		(void*)&m_freq);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Time Speed", TwType::TW_TYPE_FLOAT,	(void*)&m_speedOfTime);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Lighting", "Ambient  %", TwType::TW_TYPE_FLOAT, (void*)&m_ambientIntensity);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Lighting", "Diffuse  %", TwType::TW_TYPE_FLOAT, (void*)&m_diffuseIntensity);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Lighting", "Specular %", TwType::TW_TYPE_FLOAT, (void*)&m_specIntensity);

}

// Destroy things
void ProGenEnvironment::ShutDown()
{
	glDeleteVertexArrays(1, &m_wVAO);
	glDeleteVertexArrays(1, &m_eVAO);
	glDeleteBuffers(1, &m_wVBO);
	glDeleteBuffers(1, &m_eVBO);
	glDeleteBuffers(1, &m_wIBO);
	glDeleteBuffers(1, &m_eIBO);

	TextureHandler::UnloadAllTextures();
	ShaderHandler::UnloadAllPrograms();
}

// Update loop
void ProGenEnvironment::Update(const double a_dt)
{
	BaseApp::Update(a_dt);

	if (glfwGetKey(m_pWindow, GLFW_KEY_O))
	{
		m_treeSpawns.clear();
		CreateEnviroGrid(192);
	}
}

// Render things to screen
void ProGenEnvironment::Render()
{
	BaseApp::Render();
	RenderSkyBox();

	ApplyLightingSetup(vec3(m_ambientIntensity),
		m_sunPosition,
		m_startLightingHeight);

	m_camWeapMatrix = glm::translate(vec3(2, -1, -3)) *
		glm::rotate(90.0f, vec3(1, -1, 1)) *
		glm::scale(vec3(0.05f));

	m_camWeapModel->SetLocalTransform(m_pBaseCamera->GetWorldTransform() * m_camWeapMatrix);
	m_camWeapModel->Render();

	m_particleEmitter->SetPosition(vec3( (m_pBaseCamera->GetWorldTransform() * glm::translate(vec3(9, -7,-25))[3] ) ) );
	m_particleEmitter->Render(
		(float)GetDeltaTime(),
		(float)GetElapsedTime(),
		(float)glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_1),
		(m_pBaseCamera->GetWorldTransform()),
		m_sunPosition);

	m_sunPosition = vec3(0, 1500 * sin(GetElapsedTime()), 1500 * cos(GetElapsedTime()));
	m_sunModel->SetLocalTransform(glm::translate(m_sunPosition) * glm::scale(vec3(20)));
	m_sunModel->Render();

	TextureHandler::RenderAllTextures();

	glUseProgram(*m_enviroProg);
	int uniloc = glGetUniformLocation(*m_enviroProg, "HighestPoint");
	glUniform1f(uniloc, m_heightestVertPoint);
	
	glBindVertexArray(m_eVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_eVAO);
	glDrawElements(GL_TRIANGLES, m_eIndexCount, GL_UNSIGNED_INT, 0);
	
	glEnable(GL_BLEND);
	//
	glUseProgram(*m_waterProg);
	glBindVertexArray(m_wVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_wVAO);
	glDrawElements(GL_TRIANGLES, m_wIndexCount, GL_UNSIGNED_INT, 0);
	//
	glDisable(GL_BLEND);

	//for (uint i = 0; i < m_treeSpawns.size(); i++)
	//{
	//	if (m_treeSpawns[i][3].y > 13)
	//	{
	//		m_pineTreeModels[m_indexTypeToSpawn[i]].SetLocalTransform(m_treeSpawns[i] * glm::scale(vec3(0.025f)));
	//		m_pineTreeModels[m_indexTypeToSpawn[i]].Render();
	//	}
	//	else
	//	{
	//		m_palmTreeModels[m_indexTypeToSpawn[i]].SetLocalTransform(m_treeSpawns[i] * glm::scale(vec3(0.1f)));
	//		m_palmTreeModels[m_indexTypeToSpawn[i]].Render();
	//	}
	//}
}