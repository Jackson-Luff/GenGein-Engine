#pragma once
#include "Engine\Core\GLApplication.h"

////
// Author: Jackson Luff
// Name: Tutorial07
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Working with the frustum cube info
// * to determine if not, partually or 
// * fully visible.
////
class Tutorial07 : public GLApplication
{
public:
	// Constructors
	Tutorial07();
	Tutorial07(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial07();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

	void GetFrustumPlane( const glm::mat4& a_transform, glm::vec4* planes);
};