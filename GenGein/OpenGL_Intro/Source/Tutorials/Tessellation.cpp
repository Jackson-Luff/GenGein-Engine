#include <gl_core_4_4.h>
#include <glm/glm.hpp>
#include "Engine\GUI\AntTweak.h"
#include "Engine\Core\ShaderHandler.h"
#include "Cameras\BaseCamera.h"
#include "Tessellation.h"


Tessellation::Tessellation(c_int a_width, c_int a_height, c_pChar a_title)
	: GLApplication(a_width, a_height, a_title)
{}

Tessellation::~Tessellation()
{
}

void Tessellation::StartUp()
{
	GLApplication::StartUp();

	//Initialise camera
	InitialiseFlyCamera(5.0f, 20.0f, 0.5f,
		glm::vec3(20), glm::vec3(0));

	ShaderHandler::LoadShaderProgram("Tesselation",
		"Data/Shaders/Tess/Tess.vert",
		"Data/Shaders/Tess/Tess.frag",
		"Data/Shaders/Tess/Tess.geom",
		"Data/Shaders/Tess/Tess.tessC",
		"Data/Shaders/Tess/Tess.tessE");
	m_programID = &ShaderHandler::GetShader("Tesselation");

	TessLevelInner = 1;
	TessLevelOuter = 1;
	patchCount = 3;

	CreateIcosahedron(128);

	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Tessellation", "Inner", TW_TYPE_FLOAT, (void*)&TessLevelInner);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Tessellation", "Outer", TW_TYPE_FLOAT, (void*)&TessLevelOuter);
	m_pAntTweakGUI->AddVarRW("Main Tweaker", "Tessellation", "PatchCount", TW_TYPE_FLOAT, (void*)&patchCount);
}

void Tessellation::ShutDown()
{
}

void Tessellation::Update(const double a_dt)
{
	m_pBaseCamera->Update(a_dt);
}

void Tessellation::Render()
{
	GLApplication::Render();

	RenderSkyBox();
	
	ApplyLightingSetup(glm::vec3(0.25f), glm::vec3(0, 100 * sin(GetElapsedTime()), 100 * cos(GetElapsedTime())), 0);

	glUseProgram(*m_programID);

	float amplitude = 5.0f;
	TessLevelInner = amplitude + 1.0f + (amplitude * (float)sin(GetElapsedTime()));
	TessLevelOuter = amplitude + 1.0f + (amplitude * (float)cos(GetElapsedTime()));

	int loc = glGetUniformLocation(*m_programID, "TessLevelInner");
	glUniform1f(loc, TessLevelInner);

	loc = glGetUniformLocation(*m_programID, "TessLevelOuter");
	glUniform1f(loc, TessLevelOuter);

	glBindVertexArray(m_vao);
	glPatchParameteri(GL_PATCH_VERTICES, (int)patchCount);
	glDrawElements(GL_PATCHES, m_indexCount, GL_UNSIGNED_INT, nullptr);
}

void Tessellation::CreateIcosahedron(c_uint a_dim)
{
	const glm::vec3 Verts[] = {
		{ glm::vec3(0.000f, 0.000f, 1.000f)		* 100.0f},
		{ glm::vec3(0.894f, 0.000f, 0.447f)		* 100.0f},
		{ glm::vec3(0.276f, 0.851f, 0.447f)		* 100.0f},
		{ glm::vec3(-0.724f, 0.526f, 0.447f)	* 100.0f},
		{ glm::vec3(-0.724f, -0.526f, 0.447f)	* 100.0f},
		{ glm::vec3(0.276f, -0.851f, 0.447f)	* 100.0f},
		{ glm::vec3(0.724f, 0.526f, -0.447f)	* 100.0f},
		{ glm::vec3(-0.276f, 0.851f, -0.447f)	* 100.0f},
		{ glm::vec3(-0.894f, 0.000f, -0.447f)	* 100.0f},
		{ glm::vec3(-0.276f, -0.851f, -0.447f)	* 100.0f},
		{ glm::vec3(0.724f, -0.526f, -0.447f)	* 100.0f},
		{ glm::vec3(0.000f, 0.000f, -1.000f)	* 100.0f}	
	};
	
	const uint Faces[] = {
		2, 1, 0, 3, 2, 0, 4, 3, 0,
		5, 4, 0, 1, 5, 0, 11, 6, 7,
		11, 7, 8, 11, 8, 9, 11, 9, 10,
		11, 10, 6, 1, 2, 6, 2, 3, 7,
		3, 4, 8, 4, 5, 9, 5, 1, 10,
		2, 7, 6, 3, 8, 7, 4, 9, 8,
		5, 10, 9, 1, 6, 10 };

	m_indexCount = sizeof(Faces) / sizeof(Faces[0]);
	int vertCount = sizeof(Verts) / sizeof(Verts[0]);

	// Create the VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertCount, Verts, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* m_indexCount, Faces, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
