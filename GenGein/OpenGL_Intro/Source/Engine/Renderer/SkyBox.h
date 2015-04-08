#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>

typedef const int c_int;
typedef unsigned int uint;
typedef const std::string c_str;

class SkyBox
{
public:

	SkyBox();
	~SkyBox();

	void Create(c_str a_folderDir);
	void Render();

private:
	
	uint LoadCubeVertices(); 
	uint* m_programID;
	uint m_VAO;
	sTexture m_cubeMapTexture;
};
