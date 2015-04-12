
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "Engine\Core\GLApplication.h"
#include "Engine\Lighting.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\Core\ShaderHandler.h"
#include "Engine\GUI\AntTweak.h"
#include "Cameras\FlyCamera.h"
#include "Tutorial11.h"

using glm::vec3;
using glm::vec4;

// Default constructor
Tutorial11::Tutorial11()
{}

// Construct with specified window attributes
Tutorial11::Tutorial11(const int a_width, const int a_height, const char* a_title)
: GLApplication(a_width, a_height, a_title)
{}

// Deconstructor
Tutorial11::~Tutorial11()
{}

void Tutorial11::SetUpFBO()
{
	// Setup shadow map buffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

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

void Tutorial11::CalcLightMat()
{
	m_lightDirection = glm::normalize(glm::vec3(1, 2.5f, 1));

	glm::mat4 lightProjection = glm::ortho<float>(
		-100, 100, -100, 100, -100, 100);

	glm::mat4 lightView = glm::lookAt(m_lightDirection,
		glm::vec3(0), glm::vec3(0, 1, 0));

	m_lightMatrix = lightProjection * lightView;
}

// Initialise things
void Tutorial11::StartUp()
{
	// GLApp start up
	GLApplication::StartUp();
	
	ShaderHandler::LoadShaderProgram("UseShadow",
		"Data/Shaders/UseShadow.vert",
		"Data/Shaders/UseShadow.frag");
	m_useShadowProg = &ShaderHandler::GetShader("UseShadow");

	m_lightDirUniLoc = glGetUniformLocation(*m_useShadowProg, "lightDir");
	m_shadowMapUniLoc = glGetUniformLocation(*m_useShadowProg, "shadowMap");
	m_ulightMatUniLoc = glGetUniformLocation(*m_useShadowProg, "LightMatrix");

	ShaderHandler::LoadShaderProgram("GenShadow",
		"Data/Shaders/GenShadow.vert",
		"Data/Shaders/GenShadow.frag");
	m_genShadowProg = &ShaderHandler::GetShader("GenShadow");
	m_glightMatUniLoc = glGetUniformLocation(*m_genShadowProg, "LightMatrix");
	
	SetUpFBO();
	CalcLightMat();
	
	//Initialise camera
	InitialiseFlyCamera(5.f, 20.0f, 0.1f,
		glm::vec3(1,1,1), glm::vec3(0,0,0));

	/*
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Point X", TW_TYPE_FLOAT, (void*)&m_pPointLight->GetPosition().x);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Point Y", TW_TYPE_FLOAT, (void*)&m_pPointLight->GetPosition().y);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Point Z", TW_TYPE_FLOAT, (void*)&m_pPointLight->GetPosition().z);
	*/
	m_pBunny = new FBXModel(vec3(0));
	m_pBunny->LoadFBX(m_useShadowProg,
		"Data/Models/stanford/bunny.fbx",
		FBXFile::UNITS_CENTIMETER);

	m_pPlane = new FBXModel(vec3(0));
	m_pPlane->LoadFBX(m_useShadowProg,
		"Data/Models/plane.fbx",
		FBXFile::UNITS_CENTIMETER);
}

// Destroy things 
void Tutorial11::ShutDown()
{
}

// Updates inputs and incr time
void Tutorial11::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

void Tutorial11::UpdateShadowTexture()
{
	// Final pass: bind back-buffer and clear colour and depth
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, 4096, 4096);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_genShadowProg);
	glUniformMatrix4fv(m_glightMatUniLoc, 1, GL_FALSE, &m_lightMatrix[0][0]);
	
	m_pBunny->Render(mat4(1));
}

void Tutorial11::RenderStuff()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GetWidth(), GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_useShadowProg);

	// bind the light matrix
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

	m_pBunny->Render(mat4(1));
	m_pPlane->Render(mat4(1));
}

// Draws the stuff to screen
void Tutorial11::Render()
{
	GLApplication::Render();
	UpdateShadowTexture();
	RenderStuff();
}