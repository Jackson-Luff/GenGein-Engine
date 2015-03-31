#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;

using glm::vec4;
using glm::vec2;

class FBOMesh
{
public:
	FBOMesh();
	~FBOMesh();

	void Initialise( const uint a_desWidth, const uint a_desHeight);
	void SaveOffScreenShot();
	void Render(const glm::mat4& a_transform, 
		const int& a_AppWidth, const int& a_AppHeight );
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

	void SetUpMirror();
	uint m_VAO, m_VBO, m_IBO;
	uint m_width, m_height;
	uint m_FBOShaderID;
	int m_projViewUniLoc, m_textureUniLoc;
};