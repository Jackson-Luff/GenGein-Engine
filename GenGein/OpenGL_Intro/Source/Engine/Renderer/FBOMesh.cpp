#include <iostream>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"

#include "FBOMesh.h"

FBOMesh::FBOMesh() 
: m_VAO(0), m_VBO(0), m_IBO(0), m_FBOShaderID(0),
  m_FBO(0), m_FBOTexture(0), m_FBODepth(0),  
  m_projViewUniLoc(0), m_textureUniLoc(0),
  m_width(0), m_height(0)
{}

FBOMesh::~FBOMesh()
{
}

void FBOMesh::Initialise( const uint a_desWidth, const uint a_desHeight) 
{
	m_width = a_desWidth;
	m_height = a_desHeight;

	ShaderHandler::LoadShaderProgram("FBORenderview",
		"Data/Shaders/FBO/RenderView.vert",
		"Data/Shaders/FBO/RenderView.frag");
	m_FBOShaderID = ShaderHandler::GetShader("FBORenderview");
	CreateFBO();

	SetUpMirror();

	m_projViewUniLoc = glGetUniformLocation(m_FBOShaderID, "ProjectionView");
	m_textureUniLoc = glGetUniformLocation(m_FBOShaderID, "diffuse");
}

void FBOMesh::CreateFBO()
{
	// Setup and bind a frame-buffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	ExtractTextureFromFBO();

	if (!ValidateFBOMesh())
		return;

	// Unbind the FBO so that we can render to the back buffer
	glBindBuffer(GL_FRAMEBUFFER, 0);
}

void FBOMesh::ExtractTextureFromFBO()
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

	// while the FBO is still bound
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
}

void FBOMesh::AttachDepthBuffer()
{
	// Setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_FBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_FBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		m_width, m_height);

	// while the FBO is still bound
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, m_FBODepth);
}

bool FBOMesh::ValidateFBOMesh()
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

void FBOMesh::SetUpMirror()
{	
	VertexData vertexData[] =
	{
		{ glm::vec4(+(1), -(1), 20, 1), glm::vec2(0, 0) },
		{ glm::vec4(-(1), -(1), 20, 1), glm::vec2(1, 0) },
		{ glm::vec4(-(1), +(1), 20, 1), glm::vec2(1, 1) },
		{ glm::vec4(+(1), +(1), 20, 1), glm::vec2(0, 1) }
	};

	uint indexData[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * 4, 
				vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6,
		indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(VertexData), 0);
	//Texture Coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexData), ((char*)sizeof(glm::vec4)));
}

void FBOMesh::SaveOffScreenShot()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, m_width, m_height);

	glClearColor(0.75f, 0.75f, 0.75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBOMesh::Render(const glm::mat4& a_transform,
	const int& a_AppWidth,
	const int& a_AppHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, a_AppWidth, a_AppHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear( GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_FBOShaderID);

	// ProjectionView uniform
	glUniformMatrix4fv(m_projViewUniLoc, 1, GL_FALSE,
		&(a_transform[0][0]));

	// Texture uniform
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glUniform1i(m_textureUniLoc, 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}