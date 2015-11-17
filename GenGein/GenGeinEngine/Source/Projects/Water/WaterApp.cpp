#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Projects\Water\FrameBuffObj.h"

#include "WaterApp.h"

WaterApp::WaterApp() : BaseApp()
{}

WaterApp::WaterApp(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

WaterApp::~WaterApp()
{
	delete m_waterPlane;
}

void WaterApp::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(15.0f, 20.0f, 0.5f,
		glm::vec3(0, 5, 11), glm::vec3(0));

	ShaderHandler::LoadShaderProgram("WaterShader",
		"Data/Shaders/Testing/Water.vert",
		"Data/Shaders/Testing/Water.frag");

	m_waterPlane = new FBXModel();
	m_waterPlane->LoadFBX("WaterShader", "Data/Models/plane.fbx", FBXFile::UNITS_CENTIMETER);

	m_reflectionFBO = new FrameBuffObj();
	m_refractionFBO = new FrameBuffObj();

	m_reflectionFBO->Initialise(320, 180, "reflTexture", "WaterShader");
	m_refractionFBO->Initialise(1280, 720, "refrTexture", "WaterShader");
}

void WaterApp::ShutDown()
{
	m_waterPlane->Shutdown();
	m_reflectionFBO->Shutdown();
	m_refractionFBO->Shutdown();
}

void WaterApp::Update(const double& a_dt)
{
	BaseApp::Update(a_dt);
}

void WaterApp::Render()
{
	BaseApp::Render();
	
	//ApplyLightingSetup(glm::vec3(0),
	//	glm::vec3(0, 10, 0), 10);

	m_reflectionFBO->BindFrameBuffer();
	m_reflectionFBO->UnBindFrameBuffer(GL_TEXTURE0, glm::ivec2(GetWidth(), GetHeight()));

	m_waterPlane->Render();
}