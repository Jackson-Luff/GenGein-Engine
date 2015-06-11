#pragma once
#include <vector>
#include "Engine\Core\BaseApp.h"

typedef const unsigned int c_uint;

class FBXModel;
class TransGrid;

////
// Author: Jackson Luff
// Name: Tutorial12
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Procedural Gen
////
class ReflectionTest : public BaseApp
{
public:
	// Constructors
	ReflectionTest();
	ReflectionTest(c_int a_width, c_int a_height, const char* a_title);
	// Deconstructor
	~ReflectionTest();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double_t a_dt);
	// Render things to screen
	virtual void Render();

	// STRT SHADOW FUNC'S //
	void CalcLightingMatrix();
	void SetUpShadowRender();
	void UpdateShadowTexture();
	// END SHADOW FUNC'S //

private:
	struct VertexData
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 uv;
	};
	
	FBXModel* m_sunModel;
	std::vector<FBXModel> m_treeModels;
	
	TransGrid* m_water;
	TransGrid* m_enviro;

	// Tree1 = large tree
	// Tree2 = small tree
	// Tree3 = medium tree
	std::vector<int> m_indexTypeToSpawn;
	std::vector<glm::mat4> m_treeSpawns;

	//Environment variables
	int m_range;
};