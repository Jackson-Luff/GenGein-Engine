#include <vector>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <stb\stb_image.h>

#include "TextureHandler.h"


//Shortening
typedef std::map<c_charp, sTexture> textureMap;
textureMap TextureHandler::m_textureMap = textureMap();

TextureHandler::~TextureHandler()
{
	auto it = m_textureMap.begin();
	while (it != m_textureMap.end())
	{
		glDeleteTextures(1, &(it->second).ID);
		++it;
	}
}

// Add a texture based on desired name and directory
sTexture TextureHandler::LoadTexture(c_uint& a_prog, c_charp a_name, c_str a_dir)
{
	if (DoesTextureExist(a_name)) return m_textureMap[a_name];

	sTexture texture = sTexture();

	int imgWidth=0, imgHeight=0, imgFormat=0;
	unsigned char* data = stbi_load( a_dir.c_str(), 
		&imgWidth, &imgHeight, &imgFormat, STBI_default);

	if (data == NULL)
	{
		printf("Unable to find texture: %s thus Default assigned.\n", a_name);
		c_charp src = "Data/Objects/default.png";

		data = stbi_load(src, &imgWidth,
				&imgHeight, &imgFormat, STBI_default);
	}

	if (imgFormat == 1)
		imgFormat = GL_RED;
	if (imgFormat == 2)
		imgFormat = GL_RG;
	if (imgFormat == 3)
		imgFormat = GL_RGB;
	if (imgFormat == 4)
		imgFormat = GL_RGBA;

	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, imgFormat,
		imgWidth, imgHeight, 0, imgFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	printf("SUCCESS: %s Texture Loaded.\n", a_name);

	texture.programID = a_prog;
	texture.textureUniLoc = glGetUniformLocation(a_prog, a_name);
	m_textureMap[a_name] = texture;
	
	return texture;
}

// Add a texture based on desired name and directory
sTexture TextureHandler::LoadPerlin(c_uint& a_prog, c_charp a_name, c_uint a_dim)
{
	if (DoesTextureExist(a_name)) return m_textureMap[a_name];

	sTexture perlin;

	float *perlin_data = new float[a_dim * a_dim];
	float scale = (1.0f / a_dim) * 3;
	int octaves = 6;
	for (uint x = 0; x < a_dim; ++x)
	{
		for (uint y = 0; y < a_dim; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlin_data[y * a_dim + x] = 0.0f;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample =
					glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * a_dim + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	glGenTextures(1, &perlin.ID);
	glBindTexture(GL_TEXTURE_2D, perlin.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, a_dim, a_dim, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	perlin.textureUniLoc = glGetUniformLocation(a_prog, a_name);
	perlin.programID = a_prog;

	m_textureMap[a_name] = perlin;

	delete[] perlin_data;

	return perlin;
}

// Add a cube map desired by name and dir
sTexture TextureHandler::LoadCubeMap(uint a_prog, c_charp a_name, std::vector<c_str> a_faces)
{
	sTexture cubeMapTexture;
	glGenTextures(1, &cubeMapTexture.ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.ID);

	int width, height, format;
	unsigned char* data;

	for (uint i = 0; i < a_faces.size(); i++)
	{
		data = stbi_load(a_faces[i].c_str(), &width, &height, &format, STBI_default);

		if (data == NULL)
		{
			printf("Unable to find texture: %s thus Default assigned.\n", a_name);
			c_charp src = "Data/Objects/default.png";

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

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
			width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	cubeMapTexture.programID = a_prog;
	cubeMapTexture.textureUniLoc = glGetUniformLocation(a_prog, a_name);

	//m_cubeMap[a_name] = cubeMapTexture;

	return cubeMapTexture;
}

// Get the texture by name
sTexture TextureHandler::GetTexture(c_charp a_name)
{
	if (DoesTextureExist(a_name))
		return m_textureMap.at(a_name);
	
	printf("No texture exists for that name.");
	return sTexture();
}

// Remove a material by name
void TextureHandler::UnloadTexture(c_charp a_name)
{
	// Doesn't exist check
	if (!DoesTextureExist(a_name)) return;

	uint uiTextureID = m_textureMap[a_name].ID;

	glDeleteTextures(1, &uiTextureID);

	m_textureMap.erase(a_name);
}

// Rendering the textures
void TextureHandler::RenderAllTextures()
{
	int i = 0;
	for (auto it : m_textureMap)
	{
		if (it.second.textureUniLoc <= -1)
			continue;
	
		//Set Texture Slot
		glUseProgram(it.second.programID);
		glUniform1i(it.second.textureUniLoc, i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, it.second.ID);
		i++;
	}
}

// Rendering the textures
/*void TextureHandler::RenderAllCubeMaps()
{
	int i = 0;
	for (auto it : m_cubeMap)
	{
		if (it.second.textureUniLoc <= -1)
			continue;

		glDepthMask(GL_FALSE); 

		glUseProgram(it.second.programID);
		glBindVertexArray(it.second.VAO)
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, it.second.ID);
		
		glDepthMask(GL_TRUE);
		i++;
	}
}*/

// Check if texture exists
bool TextureHandler::DoesTextureExist(c_charp a_name)
{
	if (m_textureMap.find(a_name) == m_textureMap.end())
		return false;

	return true;
}