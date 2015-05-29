#pragma once
#include <array>
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include "Engine\Renderer\TextureHandler.h"

typedef const int c_int;
typedef unsigned int uint;
typedef const std::string c_str;

class SkyBox
{
public:

	enum SKYBOXES
	{
		SEA				= 0,
		SKY				= 1,
		SPACE			= 2,
		CHAPEL			= 3,
		SAINT_PETERS	= 4,
		YOKOHAMA		= 5,

		SIZE
	};

	enum FILETYPES
	{
		PNG = 0,
		JPG = 1
	};

	SkyBox();
	~SkyBox();

	void Create(SKYBOXES a_presetType, FILETYPES a_fileType);
	void Render(float a_condition);

private:
	void InitialiseDirs(SKYBOXES a_presetType, FILETYPES a_fileType);

	uint LoadCubeVertices(); 
	uint m_VAO;
	sTexture m_dayTexture;
	sTexture m_nightTexture;

	std::vector<std::string> m_presetDirectires;
};
