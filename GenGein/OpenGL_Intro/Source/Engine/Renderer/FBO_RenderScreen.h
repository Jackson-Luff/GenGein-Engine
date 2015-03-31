#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;

using glm::vec4;
using glm::vec2;

class FBO_RenderScreen
{
public:
	FBO_RenderScreen();
	~FBO_RenderScreen();

	void Initialise(const uint a_desWidth, const uint a_desHeight);
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
	uint m_FBO, m_FBOTexture, m_FBODepth;
	
	void SetUpViewSpace();
	uint m_VAO, m_VBO;
	uint m_width, m_height;
	uint m_FBOShaderID;
	int m_textureUniLoc;
};