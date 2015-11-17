#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\gtc\noise.hpp>

#include "Engine\Core\ShaderHandler.h"

#include "GradientNoise.h"

GradientNoise::GradientNoise()
{
}

GradientNoise::~GradientNoise()
{
	glDeleteTextures(1, &m_textureMapID);
}

void GradientNoise::GeneratePerlinTexture(const char* a_shaderName,
	uint a_textureSlot, uint a_dims, 
	uint a_octaves, float a_amp, float a_persist,
	float a_seed, const char* a_name)
{
	m_textureSlotID = a_textureSlot;
	m_programID = &ShaderHandler::GetShader(a_shaderName);
	float* perlinData = GeneratePerlinNoiseMap(a_dims, a_octaves, a_amp, a_persist, a_seed);
	BindMapToTextureBuffers(a_name, a_dims, perlinData);

	delete[] perlinData;
}

void GradientNoise::Render()
{
	int loc = m_textureSlotID - GL_TEXTURE0;

	glUseProgram(*m_programID);

	glActiveTexture(m_textureSlotID);
	glBindTexture(GL_TEXTURE_2D, m_textureMapID);
	glUniform1i(m_textureUniLoc, loc);
}

// Private Func's 

float* GradientNoise::GeneratePerlinNoiseMap(uint a_dims, uint a_octaves,
	float a_amp, float a_persist, float a_seed)
{
	float *perlin_data = new float[a_dims * a_dims];
	float scale = (1.0f / a_dims) * 6.0f;
	int octaves = 6;
	for (unsigned int x = 0; x < a_dims; ++x)
	{
		for (unsigned int  y = 0; y < a_dims; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlin_data[y * a_dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(4, (float)o);
				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq  * a_seed)			* 0.5f + 0.5f;
				freq = powf(2, (float)o);
				float perlin_sample2 = glm::perlin(vec2((float)x, (float)y) * scale/2.0f * freq * pow(a_seed,20))	* 0.5f + 0.5f;
				freq = powf(1, (float)o);
				float perlin_sample3 = glm::perlin(vec2((float)glm::floor(x), (float)glm::floor(y)) * scale * freq * pow(a_seed, -20))	* 0.5f + 0.5f;

				perlin_data[y * a_dims + x] += (perlin_sample*perlin_sample2*perlin_sample3) * amplitude;
				amplitude *= persistence;
			}
		}
	}

	return perlin_data;
}

void GradientNoise::BindMapToTextureBuffers(const char* a_name, unsigned int a_dims, float* a_gradientMap)
{
	glGenTextures(1, &m_textureMapID);
	glBindTexture(GL_TEXTURE_2D, m_textureMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, a_dims, a_dims, 0, GL_RED, GL_FLOAT, a_gradientMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_textureUniLoc = glGetUniformLocation(*m_programID, a_name);

	glBindTexture(GL_TEXTURE_2D, 0);
}
