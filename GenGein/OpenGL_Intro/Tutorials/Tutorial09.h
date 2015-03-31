#pragma once
#include <glm\glm.hpp>
#include "Engine\Core\GLApplication.h"

class FBO_RenderScreen;

////
// Author: Jackson Luff
// Name: Tutorial09
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Post-Processing effects
////
class Tutorial09 : public GLApplication
{
public:
	// Constructors
	Tutorial09();
	Tutorial09(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial09();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	
	FBO_RenderScreen* m_pRenderScreen;

	glm::vec4 initColour;
	glm::vec4 endColour;

	uint m_vao, m_vbo, m_fbo;
	uint m_fboTexture, m_fboDepth;
};