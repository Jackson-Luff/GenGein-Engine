#include <string>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <FBXFile.h>

#include "FBXModel.h"

FBXModel::FBXModel() 
	: BaseObject()
{}

FBXModel::FBXModel(vec3 a_position) 
	: BaseObject(a_position)
{}

FBXModel::~FBXModel()
{
}

bool FBXModel::LoadFBX(uint a_prog,
	std::string a_directory,
	FBXFile::UNIT_SCALE a_scale,
	bool a_loadTextures,
	bool a_loadAnimations,
	bool a_flipTextureY)
{
	m_pFbx = new FBXFile();

	if (!m_pFbx->load(a_directory.c_str(), a_scale, a_loadTextures, a_loadAnimations, a_flipTextureY))
	{
		printf("%s is an incorrect file directory\n", a_directory.c_str());
		return false;
	}
	m_pFbx->initialiseOpenGLTextures();
	CreateOpenGLBuffers();
	bonesUniform = glGetUniformLocation(a_prog, "bones");
	return true;
}

void FBXModel::CreateOpenGLBuffers()
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		// NOTE : PROBABLY NEED TO LINK PROG, VERT, FRAG

		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		// storage for the opengl data in 3 unsigned int
		uint* glData = new uint[3];
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
			mesh->m_indices.size() * sizeof(uint),
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
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i) {
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);
		delete[] glData;
	}
}

void FBXModel::Update(const double a_elapsedTime)
{
	if (m_pFbx->getSkeletonCount() > 0)
	{
		FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
		FBXAnimation* animation = m_pFbx->getAnimationByIndex(0);

		skeleton->evaluate(animation, (float)a_elapsedTime);

		for (uint bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
			skeleton->m_nodes[bone_index]->updateGlobalTransform();
	}
}

void FBXModel::Render()
{
	if (m_pFbx->getSkeletonCount() > 0)
	{
		FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
		skeleton->updateBones();

		glUniformMatrix4fv(bonesUniform, skeleton->m_boneCount,
			GL_FALSE, (float*)skeleton->m_bones);
	}

	// bind our vertex array object and draw the mesh
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);

		for (uint j = 0; j < FBXMaterial::TextureTypes_Count; j++)
		{
			if (mesh->m_material->textures[j] == nullptr)
				continue;
			
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, mesh->m_material->textures[j]->handle);
		}

		uint* glData = (uint*)mesh->m_userData;
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (uint)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);

		// delete mesh;
		// delete glData;
	}
}
