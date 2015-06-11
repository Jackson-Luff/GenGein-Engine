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

	enum SKYBOXES : uint32_t
	{
		SEA				= 0,
		SKY				= 1,
		SPACE			= 2,
		CHAPEL			= 3,
		SAINT_PETERS	= 4,
		YOKOHAMA		= 5,
		SIZE
	};

	enum FILETYPES : uint32_t
	{
		PNG = 0,
		JPG = 1
	};

	SkyBox();
	~SkyBox();

	void Create(const SKYBOXES& a_presetType, const FILETYPES& a_fileType);
	void Render(const float32_t& a_condition);

private:
	void InitialiseDirs(const SKYBOXES& a_presetType, const FILETYPES& a_fileType);

	const uint32_t LoadCubeVertices(); 
	uint32_t m_VAO;
	sTexture m_dayTexture;
	sTexture m_nightTexture;

	std::vector<const_str> m_presetDirectires;
};
