#pragma once
#include "Engine\Core\GLApplication.h"

typedef unsigned int uint;
typedef const int c_int;
typedef const unsigned int c_uint;

class Tessellation : public GLApplication
{
public:
	Tessellation(c_int a_width, c_int a_height, c_pChar a_title);
	~Tessellation();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

	void CreateIcosahedron(c_uint a_dim);

private:
	int m_indexCount;
	uint m_vao, m_vbo, m_ibo;
	uint* m_programID;

	float TessLevelInner;
	float TessLevelOuter;
	float patchCount;
};