#include <iostream>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"

#include "FBO_RenderScreen.h"

FBO_RenderScreen::FBO_RenderScreen()
	: m_VAO(0), m_VBO(0), m_FBO(0),
	m_FBOTexture(0), m_FBODepth(0),
	m_textureUniLoc(0), m_FBOShaderID(0),
	m_width(0), m_height(0)
{}

FBO_RenderScreen::~FBO_RenderScreen()
{
}

void FBO_RenderScreen::Initialise(const uint32_t& a_desWidth, const uint32_t& a_desHeight)
{
	m_width = a_desWidth;
	m_height = a_desHeight;

	SetUpViewSpace();

	CreateFBO();

	ShaderHandler::LoadShaderProgram("FBOShader",
		"Data/Shaders/FBO/RenderView.vert",
		"Data/Shaders/FBO/RenderView.frag");
	m_FBOShaderID = &ShaderHandler::GetShader("FBOShader");

	m_textureUniLoc = glGetUniformLocation(*m_FBOShaderID, "target");
}

void FBO_RenderScreen::CreateFBO()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	ExtractTextureFromFBO();

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO_RenderScreen::ExtractTextureFromFBO()
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

void FBO_RenderScreen::AttachDepthBuffer()
{
	// Setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_FBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_FBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, m_FBODepth);
}

bool FBO_RenderScreen::ValidateFBOMesh()
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

void FBO_RenderScreen::SetUpViewSpace()
{
	vec2 halfTexel = 1.0f / vec2(m_width, m_height) * 0.5f;

	VertexData vertexData[] =
	{							
		{ vec4(-(1), -(1), 0, 1), vec2(  halfTexel.x,   halfTexel.y	) },
		{ vec4(+(1), +(1), 0, 1), vec2(1-halfTexel.x, 1-halfTexel.y	) },
		{ vec4(-(1), +(1), 0, 1), vec2(  halfTexel.x, 1-halfTexel.y	) },
			  	 
		{ vec4(-(1), -(1), 0, 1), vec2(  halfTexel.x,   halfTexel.y	) },
		{ vec4(+(1), -(1), 0, 1), vec2(1-halfTexel.x,   halfTexel.y	) },
		{ vec4(+(1), +(1), 0, 1), vec2(1-halfTexel.x, 1-halfTexel.y	) }
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * 6,
		vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(VertexData), 0);
	//Texture Coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexData), (char*)offsetof(VertexData, textCoord));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FBO_RenderScreen::SaveOffScreenShot()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, m_width, m_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO_RenderScreen::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_width, m_height);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_FBOShaderID);

	// Texture uniform
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glUniform1i(m_textureUniLoc, 0);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}