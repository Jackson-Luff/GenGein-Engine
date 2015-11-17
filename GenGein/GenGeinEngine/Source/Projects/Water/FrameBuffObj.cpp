#include <iostream>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"

#include "FrameBuffObj.h"

FrameBuffObj::FrameBuffObj()
	: m_FBO(0), m_FBOTexture(0), 
	m_FBODepth(0), m_textureUniLoc(0), 
	m_FBOShaderID(0), m_width(0), m_height(0)
{}

FrameBuffObj::~FrameBuffObj()
{}

void FrameBuffObj::Initialise(const int a_desWidth, const int a_desHeight,
	const char* a_textureName, const char* a_shader)
{
	m_width = a_desWidth;
	m_height = a_desHeight;

	CreateFBO();

	if (!ValidateFBOMesh())
		return (void)printf("Not valid FBO Mesh");

	if (a_shader == NULL)
	{
		ShaderHandler::LoadShaderProgram("FBOShader",
			"Data/Shaders/FBO/RenderView.vert",
			"Data/Shaders/FBO/RenderView.frag");
		m_FBOShaderID = &ShaderHandler::GetShader("FBOShader");
	}
	else
		m_FBOShaderID = &ShaderHandler::GetShader(a_shader);

	m_textureUniLoc = glGetUniformLocation(*m_FBOShaderID, a_textureName);
}

void FrameBuffObj::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, m_width, m_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffObj::UnBindFrameBuffer(const unsigned int a_GL_TEXTURE_X, const glm::ivec2 a_viewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, a_viewport.x, a_viewport.y);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_FBOShaderID);

	// Texture uniform
	glActiveTexture(a_GL_TEXTURE_X);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glUniform1i(m_textureUniLoc, 0);
}

void FrameBuffObj::Shutdown()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_FBOTexture);	
	glDeleteRenderbuffers(1, &m_FBODepth);
}

// Private Func's

void FrameBuffObj::CreateFBO()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	ExtractTextureFromFBO();

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffObj::ExtractTextureFromFBO()
{
	// Create a texture and bind it
	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);

	// Specify texture format for storage
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attach it to the framebuffer4 as the first colour attachment
	// the FBO MUST still be bound
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						m_FBOTexture, 0);

	AttachDepthBuffer();
}

void FrameBuffObj::AttachDepthBuffer()
{
	// Setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_FBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_FBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, m_FBODepth);
}

bool FrameBuffObj::ValidateFBOMesh()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR: FRAMEBUFFER UNSUCCESSFUL.\n");
		return false;
	}

	printf("SUCCESS: FRAMEBUFFER SUCCESSFUL.\n");
	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
