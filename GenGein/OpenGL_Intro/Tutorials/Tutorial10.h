#pragma once
#include "Engine\Core\GLApplication.h"

typedef const unsigned int c_uint;
typedef unsigned int uint;
typedef const int c_int;
class FBOMesh;

////
// Author: Jackson Luff
// Name: Tutorial10
// Type: class (standard)
// Parent: GLApplication
// Description:
// * HeightMaps n ugly shit
////
class Tutorial10 : public GLApplication
{
public:
	// Constructors
	Tutorial10();
	Tutorial10(c_int a_width, c_int a_height, const char* a_title);
	// Deconstructor
	~Tutorial10();

	void CreateWaterGrid(c_uint a_rows, c_uint a_cols);
	void CreateEnviroGrid(c_uint a_rows, c_uint a_cols);

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

	void RenderWaterPlane();
	void RenderEnviroPlane();

private:

	FBOMesh* m_waterRender;

	uint* m_waterProg;
	uint* m_enviroProg;
	struct VertexData
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	// Water
	VertexData* m_waterVerts;
	uint m_waterVAO, m_waterVBO, m_waterIBO;
	uint m_waterIndexCount;
	uint m_waterRowCount;
	uint m_waterColCount;

	VertexData* m_enviroVerts;
	uint m_enviroVAO, m_enviroVBO, m_enviroIBO;
	uint m_enviroIndexCount;
	uint m_enviroRowCount;
	uint m_enviroColCount;
};