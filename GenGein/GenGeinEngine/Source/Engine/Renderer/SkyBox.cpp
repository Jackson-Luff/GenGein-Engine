#include <string>
#include <gl_core_4_4.h>
#include <stb\stb_image.h>
#include "Engine\Core\ShaderHandler.h"
#include "Skybox.h"

SkyBox::SkyBox()
{}

SkyBox::~SkyBox()
{
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	m_VAO = NULL;
	m_VBO = NULL;
}

void InitialiseDirs(const SkyBox::SKYBOXES a_presetType, std::vector<std::string>& a_dirs)
{
	const std::string fileType = ".jpg";
	const std::string names[6]
	{
		"posx", "negx",
		"posy", "negy",
		"posz", "negz",
	};

	std::string skyDir;
	
	//Setup Sea Directory
	switch (a_presetType)
	{
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
	default:
		printf("ERROR: No such directory (skybox).\n");
		return;
		break;
	}

	for (int i = 0; i < 6; i++)
		a_dirs[i] = skyDir + names[i] + fileType;
}

void SkyBox::Create(const SKYBOXES a_presetType)
{
	std::vector<std::string> faces(6);

	InitialiseDirs(a_presetType, faces);

	ShaderHandler::LoadShaderProgram("SkyBox",
		"Data/Shaders/Used/SkyBox.vert",
		"Data/Shaders/Used/SkyBox.frag");

	LoadCubeMap("SkyBox", "skybox", faces);

	faces.clear();

	LoadCubeVertices();
	
	printf("SUCCESS: SkyBox Load Successful. \n");
	printf("REMEMBER - SKYBOX is in GL Texture Slot 01 .\n\n");
}

void SkyBox::LoadCubeVertices()
{
	vec3 points[] = {
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
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

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 36, &points, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

	glBindVertexArray(0);
}

void SkyBox::LoadCubeMap(const char* a_prog, const char* a_name, std::vector<std::string> a_faces)
{
	glGenTextures(1, &m_cubeMapBufferID);
	
	int width, height, format;
	unsigned char* data;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapBufferID);
	for (unsigned int i = 0; i < a_faces.size(); i++)
	{
		width = -1, height = -1, format = -1;

		data = stbi_load(a_faces[i].c_str(), &width, &height, &format, STBI_default);

		if (data == NULL)
		{
			printf("Unable to find texture: %s thus Default assigned.\n", a_name);
			const char* src = "Data/Objects/default.png";

			data = stbi_load(src, &width, &height, &format, STBI_default);
		}

		if (format == 1)
			format = GL_RED;
		if (format == 2)
			format = GL_RG;
		if (format == 3)
			format = GL_RGB;
		if (format == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	m_pProgramID = &ShaderHandler::GetShader(a_prog);
	m_cubeMapUniLoc = glGetUniformLocation(*m_pProgramID, a_name);
}

void SkyBox::Render()
{
	glDepthMask(GL_FALSE);

	glUseProgram(*m_pProgramID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapBufferID);
	glUniform1i(m_cubeMapUniLoc, 0);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);
}