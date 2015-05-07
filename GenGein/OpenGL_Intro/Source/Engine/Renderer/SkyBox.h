#pragma once
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

	SkyBox();
	~SkyBox();

	void Create(c_str a_folderDir);
	void Render(float a_condition);

private:
	
	uint LoadCubeVertices(); 
	uint m_VAO;
	sTexture m_dayTexture;
	sTexture m_nightTexture;
};
