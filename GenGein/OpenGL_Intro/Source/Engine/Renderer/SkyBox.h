#pragma once
#include <array>
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include "Engine\Renderer\TextureHandler.h"

typedef const std::string const_str;

class SkyBox
{
public:

	enum SKYBOXES
	{
		SKY				= 1,
		SPACE			= 2,
		CHAPEL			= 3,
		SIZE
	};

	SkyBox();
	~SkyBox();

	void Create(const SKYBOXES& a_presetType);
	void Render();

private:
	void InitialiseDirs(const SKYBOXES& a_presetType);

	const unsigned int LoadCubeVertices(); 
	unsigned int vao, vbo;
	sTexture m_texture;

	std::vector<const_str> m_presetDirectires;
};
