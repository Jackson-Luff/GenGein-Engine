#pragma once
#include <glm\glm.hpp>
#include "Engine\Core\GLApplication.h"

class GPUParticleEmitter;
class FBOMesh;

////
// Author: Jackson Luff
// Name: Tutorial08
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Rendering the back buffer to meshs
////
class Tutorial08 : public GLApplication
{
public:
	// Constructors
	Tutorial08();
	Tutorial08(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial08();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	GPUParticleEmitter* m_pEmitter;
	FBOMesh* m_pFBOPlane;

	glm::vec4 initColour;
	glm::vec4 endColour;
};