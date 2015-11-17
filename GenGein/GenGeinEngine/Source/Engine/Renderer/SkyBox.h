#pragma once
#include <vector>

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

	void Create(SKYBOXES a_presetType);
	void Render();
	unsigned int& GetTextureID(){ return m_cubeMapBufferID; }

private:
	struct sTexture
	{
		unsigned int* programID;
		unsigned int TextureBuffID;
		unsigned int TextureType;
		unsigned int textureUniLoc;
	};
	
	void LoadCubeMap(const char* a_prog, const char* a_name, std::vector<std::string> a_faces);
	void LoadCubeVertices(); 
	
	unsigned int* m_pProgramID;
	unsigned int m_cubeMapUniLoc;
	unsigned int m_cubeMapBufferID;

	unsigned int m_VAO, m_VBO;
};