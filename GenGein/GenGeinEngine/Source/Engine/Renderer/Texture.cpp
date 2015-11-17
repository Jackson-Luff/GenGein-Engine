#include <gl_core_4_4.h>
#include <gl\GLU.h>
#include <stb\stb_image.h>

#include "Engine\Core\ShaderHandler.h"

#include "Texture.h"


Texture::Texture() 
	: m_programID(0), m_textureBuffID(0),
	m_textureType(0), m_textureUniLoc(0)
{}

Texture::~Texture()
{
}

void Texture::Setup(const char* a_shader, const char* a_name, const char* a_directory, unsigned int a_textureSlot, unsigned int a_textureType)
{
	m_programID = &ShaderHandler::GetShader(a_shader);
	m_textureType = a_textureType;

	int imgWidth = 0, imgHeight = 0, imgFormat = 0;
	unsigned char* data = stbi_load(a_directory, &imgWidth, &imgHeight, &imgFormat, STBI_default);

	if (data == NULL)
	{
		printf("Unable to find texture: %s thus Default assigned.\n", a_name);
		return;
	}

	if (imgFormat == 1)
		imgFormat = GL_RED;
	if (imgFormat == 2)
		imgFormat = GL_RG;
	if (imgFormat == 3)
		imgFormat = GL_RGB;
	if (imgFormat == 4)
		imgFormat = GL_RGBA;

	glGenTextures(1, &m_textureBuffID);
	glBindTexture(a_textureType, m_textureBuffID);
	glTexImage2D(a_textureType, 0, imgFormat, imgWidth, imgHeight, 0, imgFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(a_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(a_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(a_textureType);
	glTexParameterf(a_textureType, 0x84fe, 8);
	glBindTexture(a_textureType, 0);

	stbi_image_free(data);

	m_textureUniLoc = glGetUniformLocation(*m_programID, a_name);
	m_textureSlotID = a_textureSlot;

	printf("SUCCESS: %s Texture Loaded.\n", a_name);
}

void Texture::Shutdown()
{
	glDeleteTextures(1, &m_textureBuffID);
}

void Texture::Render()
{
	int loc = m_textureSlotID - GL_TEXTURE0;

	glUseProgram(*m_programID);
	glActiveTexture(m_textureSlotID);
	glBindTexture(m_textureType, m_textureBuffID);
	glUniform1i(m_textureUniLoc, loc);
}