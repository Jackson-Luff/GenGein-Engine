#include <string>
#include <gl_core_4_4.h>
#include <stb\stb_image.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Skybox.h"

SkyBox::SkyBox()
	: m_programID(0)
{}

SkyBox::~SkyBox()
{}

void SkyBox::Create(c_str a_folderDir)
{
	ShaderHandler::LoadShaderProgram("SkyBox",
		"Data/Shaders/Used/SkyBox.vert",
		"Data/Shaders/Used/SkyBox.frag");
	m_programID = &ShaderHandler::GetShader("SkyBox");

	std::vector<c_str> faces;
	faces.push_back(std::string("Data/Shaders/Used/Faces/Sky/") + "right" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Sky/") + "left" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Sky/") + "top" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Sky/") + "bottom" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Sky/") + "back" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Sky/") + "front" + ".png");
	m_dayTexture = TextureHandler::LoadCubeMap(m_programID, "skybox", faces);
	m_VAO = LoadCubeVertices();
	faces.clear();
	faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "right" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "left" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "top" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "bottom" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "back" + ".png");
	faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "front" + ".png");
	m_nightTexture = TextureHandler::LoadCubeMap(m_programID, "skybox", faces);

	printf("SUCCESS: SkyBox Load Successful.\n\n");
}

uint SkyBox::LoadCubeVertices()
{
	glm::vec3 points[] = {
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f } ,
		{ -1.0f, +1.0f, -1.0f },
								 
		{ +1.0f, -1.0f, +1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ +1.0f, -1.0f, -1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ +1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
								 
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ -1.0f, +1.0f, -1.0f },
		{ +1.0f, -1.0f, +1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ -1.0f, -1.0f, -1.0f },
								 
		{ -1.0f, +1.0f, +1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ +1.0f, -1.0f, +1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ +1.0f, -1.0f, -1.0f },
		{ +1.0f, +1.0f, -1.0f },
								 
		{ +1.0f, -1.0f, -1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ +1.0f, -1.0f, +1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ -1.0f, +1.0f, -1.0f },
								 
		{ +1.0f, +1.0f, +1.0f },
		{ -1.0f, +1.0f, -1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, -1.0f, +1.0f },
	};	

	uint vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 36, &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);

	glBindVertexArray(0);

	return vao;
}

void SkyBox::Render(float a_condition)
{
	glDepthMask(GL_FALSE);
	glUseProgram(*m_programID);
	glBindVertexArray(m_VAO);

	//glActiveTexture(GL_TEXTURE0 + 1); // use second texture image unit
	if (a_condition >= 0)
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_dayTexture.ID);
	else if (a_condition < 0)
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_nightTexture.ID);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	glDepthMask(GL_TRUE);
}