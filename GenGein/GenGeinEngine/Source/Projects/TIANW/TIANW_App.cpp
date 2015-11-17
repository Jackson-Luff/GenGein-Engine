#include "Engine\Core\ShaderHandler.h"
#include "Engine\Cameras\BaseCamera.h"
#include "Engine\Renderer\SkyBox.h"
#include "Engine\Objects\FBXModel.h"

#include "Projects\TIANW\Terrain\Terrain.h"
#include "Projects\TIANW\Water\WaterPlane.h"



#include "TIANW_App.h"

TIANW_App::TIANW_App() : BaseApp()
{}

TIANW_App::TIANW_App(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

TIANW_App::~TIANW_App()
{}

void TIANW_App::StartUp()
{
	BaseApp::StartUp();
	
	m_pSkyBox = new SkyBox();
	m_pSkyBox->Create(SkyBox::SKY);

	//Initialise camera
	InitialiseFlyCamera(1.0f, 100.0f, 0.5f,
		glm::vec3(1), glm::vec3(0));

	m_pWater = new WaterPlane();
	m_pWater->SetUp(128, 200.0);

	m_pTerrain = new Terrain();
	m_pTerrain->SetUp(512, 6.0f);

	//ShaderHandler::LoadShaderProgram("Sun",
	//	"Data/Shaders/TIANW/Sun.vert",
	//	"Data/Shaders/TIANW/Sun.frag");
	//m_pSunModel = new FBXModel();
	//m_pSunModel->LoadFBX("Sun", "Data/Models/sphere.fbx",
	//	FBXFile::UNITS_CENTIMETER);
}

void TIANW_App::ShutDown()
{
	m_pTerrain->CleanUp();
	//m_pSunModel->Shutdown();
}

void TIANW_App::Update(double a_dt)
{
	BaseApp::Update(a_dt);
}

void TIANW_App::Render()
{

	m_pSkyBox->Render();
	
	m_pTerrain->Render();
	m_pWater->Render(m_pSkyBox->GetTextureID());

	
	/*
	unsigned int dims = m_pTerrainBase->GetDimensions();
	vec3 center(dims / 2.0 * 1.5f);
	vec3 position(center);
	position.x += center.x*2.0f * sin(GetElapsedTime());
	position.z += center.z*2.0f * sin(GetElapsedTime());

	ApplyLightingSetup(position);

	m_pSunModel->Render();
	m_pSunModel->SetLocalTransform(glm::translate(position));*/
}