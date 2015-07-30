#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;

using namespace glm;

class FBO_RenderScreen
{
public:
	FBO_RenderScreen();
	~FBO_RenderScreen();

	void Initialise(const uint32_t& a_desWidth, const uint32_t& a_desHeight);
	void SaveOffScreenShot();
	void Render();
private:
	struct VertexData
	{
		vec4 position;
		vec2 textCoord;
	};

	//Components of Initialise
	void CreateFBO();
	void ExtractTextureFromFBO();
	void AttachDepthBuffer();
	bool ValidateFBOMesh();
	uint32_t m_FBO, m_FBOTexture, m_FBODepth;
	
	void SetUpViewSpace();
	uint32_t m_VAO, m_VBO;
	uint32_t m_width, m_height;
	uint32_t* m_FBOShaderID;
	int m_textureUniLoc;
};