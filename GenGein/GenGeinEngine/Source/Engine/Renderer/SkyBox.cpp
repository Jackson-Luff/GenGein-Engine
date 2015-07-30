#include <string>
#include <gl_core_4_4.h>
#include <stb\stb_image.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\TextureHandler.h"
#include "Skybox.h"

SkyBox::SkyBox()
{}

SkyBox::~SkyBox()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	vao = NULL;
	vbo = NULL;
}

void SkyBox::InitialiseDirs(const SKYBOXES& a_presetType)
{
	m_presetDirectires = std::vector<const_str>(6);

	const_str fileType = ".jpg";
	const_str names[6]
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

	for (int32_t i = 0; i < 6; i++)
		m_presetDirectires[i] = skyDir + names[i] + fileType;
}

void SkyBox::Create(const SKYBOXES& a_presetType)
{
	InitialiseDirs(a_presetType);

	ShaderHandler::LoadShaderProgram("SkyBox",
		"Data/Shaders/Used/SkyBox.vert",
		"Data/Shaders/Used/SkyBox.frag");

	std::vector<const_str> faces;

	for (uint32_t i = 0; i < 6; i++)
		faces.push_back(m_presetDirectires[i]);

	m_texture = TextureHandler::LoadCubeMap("SkyBox", "skybox", faces);
	faces.clear();

	LoadCubeVertices();
	
	printf("SUCCESS: SkyBox Load Successful.\n\n");
}

const uint SkyBox::LoadCubeVertices()
{
	vec3 points[] = {
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 36, &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);

	glBindVertexArray(0);

	return vao;
}

void SkyBox::Render()
{
	glDepthMask(GL_FALSE);
	glUseProgram(ShaderHandler::GetShader("SkyBox"));
	glBindVertexArray(vao);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture.ID);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	glDepthMask(GL_TRUE);
}