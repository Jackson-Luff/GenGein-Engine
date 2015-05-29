#include <string>
#include <gl_core_4_4.h>
#include <stb\stb_image.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Skybox.h"

SkyBox::SkyBox()
{}

SkyBox::~SkyBox()
{}

void SkyBox::InitialiseDirs(SKYBOXES a_presetType, FILETYPES a_type)
{
	m_presetDirectires = std::vector<std::string>(SIZE);

	const std::string names[6]
	{
		"posx", "negx",
		"posy", "negy",
		"posz", "negz",
	};

	const std::string fileTypes[2] {
		".png", ".jpg"
	};

	const std::string fileType = fileTypes[a_type];

	std::string skyDir;
	
	//Setup Sea Directory
	switch (a_presetType)
	{
	case SkyBox::SEA:
		skyDir = "Data/SkyBox/Sea/";
		break;
	case SkyBox::SKY:
		skyDir = "Data/SkyBox/Sky/";
		break;
	case SkyBox::SPACE:
		skyDir = "Data/SkyBox/Space/";
		break;
	case SkyBox::CHAPEL:
		skyDir = "Data/SkyBox/Chapel/";
		break;
	case SkyBox::SAINT_PETERS:
		skyDir = "Data/SkyBox/SaintPeters/";
		break;
	case SkyBox::YOKOHAMA:
		skyDir = "Data/SkyBox/Yokohama/";
		break;
	default:
		return;
		break;
	}

	for (int i = 0; i < 6; i++)
		m_presetDirectires[i] = skyDir + names[i] + fileType;
}

void SkyBox::Create(SKYBOXES a_presetType, FILETYPES a_fileType)
{
	InitialiseDirs(a_presetType, a_fileType);

	ShaderHandler::LoadShaderProgram("SkyBox",
		"Data/Shaders/Used/SkyBox.vert",
		"Data/Shaders/Used/SkyBox.frag");

	std::vector<c_str> faces;

	for (uint i = 0; i < 6; i++)
		faces.push_back(m_presetDirectires[i]);

	m_dayTexture = TextureHandler::LoadCubeMap("SkyBox", "skybox", faces);
	m_VAO = LoadCubeVertices();
	faces.clear();
	//faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "posx" + ".jpg");
	//faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "negx" + ".jpg");
	//faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "posy" + ".jpg");
	//faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "negy" + ".jpg");
	//faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "posz" + ".jpg");
	//faces.push_back(std::string("Data/Shaders/Used/Faces/Space/") + "negz" + ".jpg");
	//m_nightTexture = TextureHandler::LoadCubeMap("SkyBox", "skybox", faces);

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
	glUseProgram(ShaderHandler::GetShader("SkyBox"));
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