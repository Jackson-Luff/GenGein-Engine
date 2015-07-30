#include <vector>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <stb\stb_image.h>
#include <FBXFile.h>
#include "Engine\Core\ShaderHandler.h"

#include "TextureHandler.h"

//Shortening
using TextureMap = std::vector<std::pair<const_pChar, sTexture>>;
TextureMap TextureHandler::m_textureMap = TextureMap();

// Add a texture based on desired name and directory
const sTexture TextureHandler::LoadTexture(const_pChar a_prog, const_pChar a_name, const_str& a_dir)
{
	if (DoesTextureExist(a_name)) 
		return m_textureMap[GetTextureIndex(a_name)].second;

	sTexture texture = sTexture();

	int32_t imgWidth = 0, imgHeight = 0, imgFormat = 0;
	unsigned char* data = stbi_load(a_dir.c_str(),
		&imgWidth, &imgHeight, &imgFormat, STBI_default);

	if (data == NULL)
	{
		printf("Unable to find texture: %s thus Default assigned.\n", a_name);
		const_pChar src = "Data/Objects/default.png";

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

	texture.programID = &ShaderHandler::GetShader(a_prog);
	texture.textureUniLoc = glGetUniformLocation(*texture.programID, a_name);
	m_textureMap.push_back({ a_name, texture });

	return texture;
}

// Add a texture based on desired name and directory
const sTexture TextureHandler::LoadPerlin(const_pChar a_prog, const_pChar a_name, const uint32_t& a_dim)
{
	if (DoesTextureExist(a_name)) 
		return m_textureMap[GetTextureIndex(a_name)].second;

	sTexture perlin;

	float32_t *perlin_data = new float32_t[a_dim * a_dim];
	float32_t scale = (1.0f / a_dim) * 3;
	int32_t octaves = 6;
	for (uint32_t x = 0; x < a_dim; ++x)
	{
		for (uint32_t y = 0; y < a_dim; ++y)
		{
			float32_t amplitude = 1.f;
			float32_t persistence = 0.3f;
			perlin_data[y * a_dim + x] = 0.0f;
			for (int32_t o = 0; o < octaves; ++o)
			{
				float32_t freq = powf(2, (float32_t)o);
				float32_t perlin_sample =
					glm::perlin(glm::vec2((float32_t)x, (float32_t)y) * scale * freq) * 0.5f + 0.5f;
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

	perlin.programID = &ShaderHandler::GetShader(a_prog);
	perlin.textureUniLoc = glGetUniformLocation(*perlin.programID, a_name);

	m_textureMap.push_back({ a_name, perlin });

	delete[] perlin_data;

	return perlin;
}

// Add a cube map desired by name and dir
const sTexture TextureHandler::LoadCubeMap(const_pChar a_prog, const_pChar a_name, std::vector<const_str>& a_faces)
{
	sTexture cubeMapTexture;
	glGenTextures(1, &cubeMapTexture.ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.ID);

	int width, height, format;
	unsigned char* data;

	for (unsigned int i = 0; i < a_faces.size(); i++)
	{
		width = 0, height = 0, format = 0;

		data = stbi_load(a_faces[i].c_str(), &width, &height, &format, STBI_default);

		if (data == NULL)
		{
			printf("Unable to find texture: %s thus Default assigned.\n", a_name);
			const_pChar src = "Data/Objects/default.png";

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

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	cubeMapTexture.programID = &ShaderHandler::GetShader(a_prog);
	cubeMapTexture.textureUniLoc = glGetUniformLocation(*cubeMapTexture.programID, a_name);
	m_textureMap.push_back({ a_name, cubeMapTexture });

	return cubeMapTexture;
}

const void TextureHandler::LoadFBXTexture(const_pChar a_prog, const FBXMaterial* a_mat)
{
	//TODO: Make this a FBXMaterial** to point to the address of the matieral.

	const_pChar types[] =
	{
		"diffuseMap",
		"ambientMap",
		"glowMap",
		"specularMap",
		"glossMap",
		"emissiveMap",
		"normalMap",
		"alphaMap",
		"displacementMap",
		"decalMap"
	};

	for (uint32_t i = 0; i < FBXMaterial::TextureTypes_Count; i++)
	{
		if (a_mat->textures[i] == nullptr)
			continue;

		if (DoesTextureExist(a_mat->textures[i]->path.c_str())) 
			return;

		sTexture texture = sTexture();
		
		texture.ID = a_mat->textures[i]->handle;
		texture.programID = &ShaderHandler::GetShader(a_prog);
		texture.textureUniLoc = glGetUniformLocation(*texture.programID, types[i]);
		m_textureMap.push_back({ a_mat->textures[i]->path.c_str(), texture });
	}
}

const uint32_t TextureHandler::LoadFrameBuffer(const_pChar a_prog, const_pChar a_name, const int32_t& a_width, const int32_t& a_height)
{
	sTexture FBO;
	uint32_t m_fbo, m_fboDepth;

	// Setup and bind a framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &FBO.ID);
	glBindTexture(GL_TEXTURE_2D, FBO.ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, a_width, a_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FBO.ID, 0);

	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		a_width, a_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, m_fboDepth);

	uint32_t drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
		return 0;
	}

	// unbind the FBO so that we can render to the back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FBO.programID = &ShaderHandler::GetShader(a_prog);
	FBO.textureUniLoc = glGetUniformLocation(*FBO.programID, a_name);
	m_textureMap.push_back({ a_name, FBO });

	return m_fbo;
}

// Get the texture by name
const sTexture TextureHandler::GetTexture(const_pChar a_name)
{
	if (DoesTextureExist(a_name))
	{
		for (auto pair : m_textureMap)
		{
			if (pair.first == a_name)
				return pair.second;
		}
	}

	printf("No texture exists for that name.");
	return sTexture();
}

const int32_t TextureHandler::GetTextureIndex(const_pChar a_name)
{
	if (DoesTextureExist(a_name))
	{
		for (uint32_t i = 0; i < m_textureMap.size(); i++)
		{
			if (m_textureMap[i].first == a_name)
				return i;
		}
	}

	printf("No texture exists for that name.");
	return -1;
}

// Remove a material by name
const void TextureHandler::UnloadTexture(const_pChar a_name)
{
	sTexture texture = GetTexture(a_name);
	
	glDeleteTextures(1, &texture.ID);
}

const void TextureHandler::UnloadAllTextures()
{
	for (auto& it : m_textureMap)
	{
		glDeleteTextures(1, &(it.second).ID);
	}

	m_textureMap.clear();
}

// Rendering the textures
const void TextureHandler::RenderAllTextures()
{
	int32_t i = 0;
	for (auto& it : m_textureMap)
	{
		if (it.second.textureUniLoc <= -1)
			continue;

		//Set Texture Slot
		glUseProgram(*it.second.programID);
		glUniform1i(it.second.textureUniLoc, i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, it.second.ID);
		i++;
	}
}

// Check if texture exists
const bool TextureHandler::DoesTextureExist(const_pChar a_name)
{
	for (auto pair : m_textureMap)
	{
		if (pair.first == a_name)
			return true;
	}
	
	return false;
}