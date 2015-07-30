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
		const std::string a_directory,
		const FBXFile::UNIT_SCALE a_scale = FBXFile::UNITS_METER,
		const bool a_loadTextures = true,
		const bool a_loadAnimations = true,
		const bool a_flipTextureY = true);

	bool LoadFBX(
		const std::string a_directory,
		const FBXFile::UNIT_SCALE a_scale = FBXFile::UNITS_METER,
		bool a_loadTextures = true,
		bool a_loadAnimations = true,
		bool a_flipTextureY = true);
	
	// Returns found mesh count
	inline const uint32_t getMeshCount() const 
	{ return m_pFbx->getMeshCount(); }
	// Returns Mesh based on index element
	FBXMeshNode* const getMeshByIndex(const uint32_t& a_index) const 
	{ return m_pFbx->getMeshByIndex(a_index); }
	
	void Shutdown();
	void Update(const double_t& a_dt);
	void Render();

private:
	void CreateOpenGLBuffers();
	void CleanupOpenGLBuffers();

	void* m_userData;

	int32_t m_bonesUniform;

	FBXFile* m_pFbx;
	FBXMaterial* m_material;
	std::vector<FBXVertex> m_vertices;
};