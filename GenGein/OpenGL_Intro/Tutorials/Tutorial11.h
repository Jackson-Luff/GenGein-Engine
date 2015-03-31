#pragma once
#include "Engine\Core\GLApplication.h"

typedef const unsigned int c_uint;
typedef unsigned int uint;
typedef const int c_int;

class FBXModel;
class Lighting;

////
// Author: Jackson Luff
// Name: Tutorial11
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Shadows!
////
class Tutorial11 : public GLApplication
{
public:
	// Constructors
	Tutorial11();
	Tutorial11(c_int a_width, c_int a_height, const char* a_title);
	// Deconstructor
	~Tutorial11();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

	void SetUpFBO();
	void CalcLightMat();

	void UpdateShadowTexture();
	void RenderStuff();

private:
	uint m_useShadowProg;
	uint m_genShadowProg;

	FBXModel* m_pBunny;
	FBXModel* m_pPlane;

	Lighting* m_pAmbientLight;
	Lighting* m_pPointLight;

	uint m_FBO, m_FBODepth;

	int m_glightMatUniLoc,
		m_ulightMatUniLoc,
		m_lightDirUniLoc,
		m_shadowMapUniLoc;

	glm::vec3 m_lightDirection;
	glm::mat4 m_lightMatrix;
};