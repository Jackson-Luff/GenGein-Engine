#pragma once
#include <vector>
#include "Engine\Core\GLApplication.h"

typedef const unsigned int c_uint;

class ObjMesh;

////
// Author: Jackson Luff
// Name: Tutorial12
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Procedural Gen
////
class Tutorial12 : public GLApplication
{
public:
	// Constructors
	Tutorial12();
	Tutorial12(c_int a_width, c_int a_height, const char* a_title);
	// Deconstructor
	~Tutorial12();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

	void CreatePerlinPlane(c_uint a_dim);
	void UpdatePerlin( const glm::vec4& camWorldMat);
	void CreateEnviroGrid(c_uint a_dim);
private:
	struct VertexData
	{
		glm::vec4 position;
		glm::vec2 uv;
	};

	uint m_VAO, m_indexCount, m_perlinTexture;
	int perlinTextureUniLoc;
	float* m_perlin_data;
	uint m_dims;

	// Enviro progrma

	uint* m_enviroProg;
	uint m_enviroIndexCount, m_enviroVAO;

	ObjMesh* m_palmTree;
	std::vector<glm::mat4> m_treeSpawns;

	//Environment variables
	float m_amplitude;
	float m_seeder;
};