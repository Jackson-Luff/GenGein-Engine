#pragma once
#include <glm\glm.hpp>
#include <FBXFile.h>
#include "BaseObject.h"

typedef unsigned int uint;

class FBXModel : public BaseObject
{
public:
	FBXModel();
	~FBXModel();

	bool LoadFBX(
		const char* a_shaderName,
		std::string a_directory,
		FBXFile::UNIT_SCALE a_scale = FBXFile::UNITS_METER,
		bool a_loadTextures = true,
		bool a_loadAnimations = true,
		bool a_flipTextureY = true);

	bool LoadFBX(
		std::string a_directory,
		FBXFile::UNIT_SCALE a_scale = FBXFile::UNITS_METER,
		bool a_loadTextures = true,
		bool a_loadAnimations = true,
		bool a_flipTextureY = true);
	
	// Returns found mesh count
	inline uint getMeshCount() const {
		return m_pFbx->getMeshCount();
	}
	// Returns Mesh based on index element
	FBXMeshNode* getMeshByIndex(uint a_index) const {
		return m_pFbx->getMeshByIndex(a_index);
	}
	
	void CleanupOpenGLBuffers();

	void Update(const double a_dt);
	void Render();
private:
	void CreateOpenGLBuffers();

	int m_bonesUniform;
	FBXMaterial* m_material;
	std::vector<FBXVertex> m_vertices;
	void* m_userData;

	FBXFile* m_pFbx;
};