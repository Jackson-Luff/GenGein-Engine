#include "Engine\Core\ShaderHandler.h"
#include "Engine\Objects\FBXModel.h"
#include "Projects\Water\FrameBuffObj.h"
#include "Engine\Input\InputHandle.h"

#include "PatternApp.h"

PatternApp::PatternApp() : BaseApp()
{}

PatternApp::PatternApp(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

PatternApp::~PatternApp()
{
	delete m_patternPlane;
}

void PatternApp::StartUp()
{
	BaseApp::StartUp();

	//Initialise camera
	InitialiseFlyCamera(15.0f, 20.0f, 0.5f,
		glm::vec3(0, 5, 11), glm::vec3(0));

	ShaderHandler::LoadShaderProgram("PatternShader",
		"Data/Shaders/Testing/Patterns.vert",
		"Data/Shaders/Testing/Patterns.frag");

	m_patternPlane = new FBXModel();
	m_patternPlane->LoadFBX("PatternShader", "Data/Models/plane.fbx", FBXFile::UNITS_CENTIMETER);

	ShaderHandler::LoadShaderProgram("PostDisplayShader",
		"Data/Shaders/Testing/Display.vert",
		"Data/Shaders/Testing/Display.frag");

	m_displayPlane = new FBXModel();
	m_displayPlane->LoadFBX("PostDisplayShader", "Data/Models/plane.fbx", FBXFile::UNITS_CENTIMETER);

	m_postProcessEffect = new FrameBuffObj();
	m_postProcessEffect->Initialise(GetWidth(), GetHeight(), "ScreenPrintTexture", "PostDisplayShader");
}

void PatternApp::ShutDown()
{
	m_patternPlane->Shutdown();
	m_displayPlane->Shutdown();
	m_postProcessEffect->Shutdown();
}

void PatternApp::Update(const double& a_dt)
{
	BaseApp::Update(a_dt);
}

void PatternApp::Render()
{
	BaseApp::Render();
	
	//ApplyLightingSetup(glm::vec3(0),
	//	glm::vec3(0, 10, 0), 10);

	m_postProcessEffect->BindFrameBuffer();
	m_patternPlane->Render();
	m_postProcessEffect->UnBindFrameBuffer(GL_TEXTURE0, ivec2(GetWidth(), GetHeight()));
	
	m_displayPlane->Render();
}