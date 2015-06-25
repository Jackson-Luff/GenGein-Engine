#pragma once
#include "Engine\Core\BaseApp.h"

using namespace glm;

class Tessellation : public BaseApp
{
public:
	Tessellation(const int32_t& a_width, const int32_t& a_height, const char* a_title);
	~Tessellation();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double_t& a_dt);
	// Render things to screen
	virtual void Render();

	void CreateIcosahedron(const uint32_t& a_dim);

private:
	int32_t m_indexCount;
	uint32_t m_vao, m_vbo, m_ibo;
	uint32_t m_programID;

	float_t TessLevelInner;
	float_t TessLevelOuter;
	float_t patchCount;
};