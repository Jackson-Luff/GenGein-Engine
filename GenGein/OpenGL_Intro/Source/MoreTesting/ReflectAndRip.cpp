
#include <gl_core_4_4.h>
#include "Cameras\BaseCamera.h"
#include "Engine\TransGrid.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"

#include "ReflectAndRip.h"

ReflectAndRip::ReflectAndRip()
{
}

ReflectAndRip::ReflectAndRip(c_int a_width, c_int a_height, c_pChar a_title)
	: GLApplication(a_width, a_height, a_title)
{
}

ReflectAndRip::~ReflectAndRip()
{
}

void ReflectAndRip::StartUp()
{
	GLApplication::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(397.0f, 5.0f, 0.5f), glm::vec3(0, 0, 397.0f));

	ShaderHandler::LoadShaderProgram("ReflectSphere",
		"Data/Shaders/Used/ReflectStuff.vert",
		"Data/Shaders/Used/ReflectStuff.frag");

	m_sphere = new FBXModel();
	m_sphere->LoadFBX("ReflectSphere",
		"Data/Models/sphere.fbx",
		FBXFile::UNITS_CENTIMETER);

	m_waterPlane = new TransGrid();
	m_waterPlane->SetUpShaderProgram("Water",
		"Data/Shaders/Used/Water.vert",
		"Data/Shaders/Used/Water.frag");
	m_waterPlane->GenQuad(4000, false, false);

	m_fbo = TextureHandler::LoadFrameBuffer("Water", "reflectTexture", 4000, 4000);
}

void ReflectAndRip::ShutDown()
{
}

void ReflectAndRip::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

void ReflectAndRip::Render()
{
	GLApplication::Render();

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 currView = m_pBaseCamera->GetView();

	//int count = 11;
	//for (int i = 0; i < count; i++)
	//{
	//	for (int j = 0; j < count; j++)
	//	{
	//		float offset = 40;
	//		float scale = 1.5f;
	//		vec3 pos = vec3(offset * i, 10, offset * j);
	//
	//		if (i == 0 || j == 0 || i == count - 1 || j == count - 1)
	//			scale = 6.0f;
	//
	//		m_sphere->SetLocalTransform(pos, 0, vec3(1), vec3(scale));
	//		m_sphere->Render();
	//	}
	//}
	

	//m_pBaseCamera->SetView();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	RenderSkyBox();

	ApplyLightingSetup(glm::vec3(0.25),
		glm::vec3(200, 500, 0), 10);

	TextureHandler::RenderAllTextures();

	//int count = 11;
	//for (int i = 0; i < count; i++)
	//{
	//	for (int j = 0; j < count; j++)
	//	{
	//		float offset = 40;
	//		float scale = 1.5f;
	//		vec3 pos = vec3(offset * i, 10, offset * j);
	//			
	//		if (i == 0 || j == 0 || i == count - 1 || j == count - 1)
	//			scale = 6.0f;
	//
	//		m_sphere->SetLocalTransform(pos, 0, vec3(1), vec3(scale));
	//		m_sphere->Render();
	//	}
	//}

	glEnable(GL_BLEND);
	m_waterPlane->Render();
	glDisable(GL_BLEND);
}
