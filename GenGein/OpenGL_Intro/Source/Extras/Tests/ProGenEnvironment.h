#pragma once
#include <vector>
#include "Engine\Core\BaseApp.h"

typedef const unsigned int c_uint;

class FBXModel;
class GPUParticleEmitter;

////
// Author: Jackson Luff
// Name: Tutorial12
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Procedural Gen
////
class ProGenEnvironment : public BaseApp
{
public:
	// Constructors
	ProGenEnvironment();
	ProGenEnvironment(c_int a_width, c_int a_height, const char* a_title);
	// Deconstructor
	~ProGenEnvironment();

	// Builds a segmented plane for the water
	void CreateWaterGrid(c_uint a_dim);
	// Calculates diamond-square for sharp normals
	void CalcDiamondSquare(glm::vec4 a_currPos, uint a_dim);
	// Builds a segmented plane for the land
	// Also has in-built height mapping and normal cal's
	void CreateEnviroGrid(c_uint a_dim);
	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:

	struct VertexData
	{
		glm::vec4 position;
		glm::vec2 uv;
	};
	
	// Enviro program
	uint* m_enviroProg;
	uint* m_waterProg;

	uint m_wIndexCount, m_wVAO, m_wVBO, m_wIBO;
	uint m_eIndexCount, m_eVAO, m_eVBO, m_eIBO;

	GPUParticleEmitter* m_particleEmitter;
	FBXModel* m_sunModel;
	FBXModel* m_camWeapModel;
	std::vector<FBXModel*> m_pineTreeModels;
	std::vector<FBXModel*> m_palmTreeModels;
	glm::mat4 m_camWeapMatrix;

	// Tree1 = large tree
	// Tree2 = small tree
	// Tree3 = medium tree
	std::vector<int> m_indexTypeToSpawn;
	std::vector<glm::mat4> m_treeSpawns;

	//Environment variables
	float m_range;
	float m_amplitude;
	float m_seeder;
	float m_persistence;
	float m_octaves;
	float m_freq;

	float m_startLightingHeight;
	float m_ambientIntensity;
	float m_diffuseIntensity;
	float m_specIntensity;
	float m_speedOfTime;
	float m_heightestVertPoint;

	//uint m_FrameBuffObj;
};