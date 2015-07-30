#include <string>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include "Engine\Renderer\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"
#include <FBXFile.h>

#include "FBXModel.h"

FBXModel::FBXModel()
: BaseObject()
{}

FBXModel::~FBXModel()
{
	CleanupOpenGLBuffers();
}

bool FBXModel::LoadFBX(
	const char* a_shaderName,
	const std::string a_directory,
	const FBXFile::UNIT_SCALE a_scale,
	const bool a_loadTextures,
	const bool a_loadAnimations,
	const bool a_flipTextureY)
{
	m_programID = &ShaderHandler::GetShader(a_shaderName);

	m_pFbx = new FBXFile();

	if (!m_pFbx->load(a_directory.c_str(), a_scale, a_loadTextures, a_loadAnimations, a_flipTextureY))
	{
		printf("%s is an incorrect file directory\n", a_directory.c_str());
		return false;
	}
	m_pFbx->initialiseOpenGLTextures();
	CreateOpenGLBuffers();

	for (uint32_t i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		uint32_t* glData = (uint32_t*)mesh->m_userData;

		// call TextureLoad here
		TextureHandler::LoadFBXTexture(a_shaderName, mesh->m_material);
	}

	m_localMatUniLoc = glGetUniformLocation(*m_programID, "LocalMatrix");
	m_bonesUniform = glGetUniformLocation(*m_programID, "bones");

	return true;
}

bool FBXModel::LoadFBX(
	const std::string a_directory,
	const FBXFile::UNIT_SCALE a_scale,
	const bool a_loadTextures,
	const bool a_loadAnimations,
	const bool a_flipTextureY)
{
	ShaderHandler::LoadShaderProgram("FBXProgram",
		"Data/Shaders/Used/FbxShader.vert",
		"Data/Shaders/Used/FbxShader.frag");
	m_programID = &ShaderHandler::GetShader("FBXProgram");

	m_pFbx = new FBXFile();

	if (!m_pFbx->load(a_directory.c_str(), a_scale, a_loadTextures, a_loadAnimations, a_flipTextureY))
	{
		printf("%s is an incorrect file directory\n", a_directory.c_str());
		return false;
	}

	m_pFbx->initialiseOpenGLTextures();
	CreateOpenGLBuffers();

	// bind our vertex array object and draw the mesh
	for (uint32_t i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		uint32_t* glData = (uint32_t*)mesh->m_userData;

		// Extracts textures from material and references data from
		// Already stored data
		TextureHandler::LoadFBXTexture("FBXProgram", mesh->m_material);
	}

	m_localMatUniLoc = glGetUniformLocation(*m_programID, "LocalMatrix");
	m_bonesUniform = glGetUniformLocation(*m_programID, "bones");
	return true;
}

void FBXModel::Shutdown()
{
	CleanupOpenGLBuffers();
}

void FBXModel::Update(const double_t& a_elapsedTime)
{
	if (m_pFbx->getSkeletonCount() > 0)
	{
		FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
		FBXAnimation* animation = m_pFbx->getAnimationByIndex(0);

		skeleton->evaluate(animation, (float)a_elapsedTime);

		for (uint32_t bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
			skeleton->m_nodes[bone_index]->updateGlobalTransform();
	}
}

void FBXModel::Render()
{
	glUseProgram(*m_programID);

	if (m_pFbx->getSkeletonCount() > 0)
	{
		FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
		skeleton->updateBones();

		glUniformMatrix4fv(m_bonesUniform, skeleton->m_boneCount,
			GL_FALSE, (float*)skeleton->m_bones);
	}

	// bind our vertex array object and draw the mesh
	for (uint32_t i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		uint32_t* glData = (uint32_t*)mesh->m_userData;

		glUniformMatrix4fv(m_localMatUniLoc, 1, GL_FALSE, &m_localTrans[0][0]);

		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);

		// delete mesh;
		// delete glData;
	}
}

// Private Func's:

void FBXModel::CreateOpenGLBuffers()
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (uint32_t i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		// NOTE : PROBABLY NEED TO LINK PROG, VERT, FRAG

		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		// storage for the opengl data in 3 unsigned int
		uint32_t* glData = new uint32_t[3];
		// more gl shit;
		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);
		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);
		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);
		glBufferData(GL_ARRAY_BUFFER,
			mesh->m_vertices.size() * sizeof(FBXVertex),
			mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->m_indices.size() * sizeof(uint32_t),
			mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // Positions
		glEnableVertexAttribArray(1); // Colours
		glEnableVertexAttribArray(2); // Normals
		glEnableVertexAttribArray(3); // Tangents
		glEnableVertexAttribArray(4); // BiNormals
		glEnableVertexAttribArray(5); // Indices
		glEnableVertexAttribArray(6); // Weights
		glEnableVertexAttribArray(7); // Texture Coords

		// Positions
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::PositionOffset);
		// Colours
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::ColourOffset);
		// Normals
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);
		// Tangents
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::TangentOffset);
		// BiNormals
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::BiNormalOffset);
		// Indices
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::IndicesOffset);
		// Weights
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::WeightsOffset);
		// Texture Coordinates
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		mesh->m_userData = glData;

		// delete mesh;
		// delete glData;
	}
}

void FBXModel::CleanupOpenGLBuffers()
{
	// clean up the vertex data attached to each mesh
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		unsigned int* glData = (unsigned int*)m_pFbx->getMeshByIndex(i)->m_userData;
		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);
		delete[] glData;
	}
}
