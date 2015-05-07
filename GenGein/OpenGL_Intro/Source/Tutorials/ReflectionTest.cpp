#include <stack>
#include <gl_core_4_4.h>
#include <glm\ext.hpp>
#include "Cameras\BaseCamera.h"
#include "Engine\GUI\AntTweak.h"
#include "Engine\TransGrid.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "ReflectionTest.h"

ReflectionTest::ReflectionTest()
{
}

// Construct with specified window attributes
ReflectionTest::ReflectionTest(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

ReflectionTest::~ReflectionTest()
{
}

// Loads in the tree's that are randomly sampled 
void LoadTrees(std::vector< FBXModel>& a_trees)
{
	for (int i = 1; i < 4; i++)
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
void ReflectionTest::StartUp()
{
	GLApplication::StartUp();

	m_range = 100;

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(218, 146, 167), glm::vec3(0, 0, 0));

	m_water = new TransGrid();
	m_water->SetUpShaderProgram("WaterMap",
		"Data/Shaders/Used/Water.vert",
		"Data/Shaders/Used/Water.frag");
	m_water->GenQuad(400, false, false);

	m_enviro = new TransGrid();
	m_enviro->SetUpShaderProgram("EnviroMap",
		"Data/Shaders/Used/Enviro.vert",
		"Data/Shaders/Used/Enviro.frag",
		"Data/Shaders/Used/Enviro.geom");
	m_enviro->GenSegmentedGrid(192, 1.92f, true, false);
	
	TextureHandler::LoadPerlin("EnviroMap", "heightMap", 512);
	TextureHandler::LoadTexture("EnviroMap", "SandMap", "Data/Textures/sand_tile.jpg");
	TextureHandler::LoadTexture("EnviroMap", "GrassMap", "Data/Textures/grass_tiled.tga");
	TextureHandler::LoadTexture("EnviroMap", "StoneMap", "Data/Textures/dirt_tiled.tga");

	m_sunModel = new FBXModel();
	m_sunModel->LoadFBX(
		"SunPlane",
		"Data/Models/plane.fbx",
		FBXFile::UNITS_CENTIMETER);
	m_sunModel->SetPosition(vec3(0, 300, 0));

	//LoadTrees(m_treeModels);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Environment", "Population", TwType::TW_TYPE_INT16, (void*)&m_range);
}

// Destroy things
void ReflectionTest::ShutDown()
{
}

// Update loop
void ReflectionTest::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);

	if (glfwGetKey(m_pWindow, GLFW_KEY_O))
	{
		m_treeSpawns.clear();
	}
}

// Render things to screen
void ReflectionTest::Render()
{
	GLApplication::Render();

	m_sunPosition = vec3(0.0f, 500.0f, 500.0f);

	m_sunModel->SetPosition(m_sunPosition);
	m_sunModel->Render();

	TextureHandler::RenderAllTextures();
	
	m_enviro->Render();

	glEnable(GL_BLEND);

	m_water->Render();

	//vec3 treeScale = vec3(0.025f);
	//for (uint i = 0; i < m_treeSpawns.size(); i++)
	//{
	//	if (m_treeSpawns[i][3].y > 13)
	//	{
	//		m_treeModels[m_indexTypeToSpawn[i]].SetLocalTransform(m_treeSpawns[i] * glm::scale(treeScale));
	//		m_treeModels[m_indexTypeToSpawn[i]].Render();
	//	}
	//}
	glDisable(GL_BLEND);
}