#pragma once
#include <glm\glm.hpp>

typedef unsigned int uint;

class FrameBuffObj
{
public:
	FrameBuffObj();
	~FrameBuffObj();

	void Initialise(const int a_desWidth = 800, const int a_desHeight = 600,
		const char* a_textureName = NULL, const char* a_shader = NULL);
	void BindFrameBuffer();
	void UnBindFrameBuffer(const unsigned int a_GL_TEXTURE_X, const glm::ivec2 a_viewport);

	void Shutdown();
private:
	struct VertexData
	{
		glm::vec4 position;
		glm::vec2 textCoord;
	};

	//Components of Initialise
	void CreateFBO();
	void ExtractTextureFromFBO();
	void AttachDepthBuffer();
	bool ValidateFBOMesh();

	unsigned int m_FBO, m_FBOTexture, m_FBODepth;
	unsigned int m_width, m_height;
	unsigned int* m_FBOShaderID;
	int m_textureUniLoc;
};