#pragma once
#include "Engine\Core\GLApplication.h"

////
// Author: Jackson Luff
// Name: Tutorial01
// Type: class (standard)
// Parent: GLApplication
// Description:
// * In this Tutorial01 we render a plane 
// * whilst learning OpenGL buffers, programs
// * and shaders.
////
class Tutorial01 : public GLApplication
{
public:
	// Constructors
	Tutorial01();
	Tutorial01( const int a_width, const int a_height, const char* a_title);
	// Deconstructors
	~Tutorial01();

	// Initialise Things
	void StartUp();
	// Destroy things
	void ShutDown();

	// Update loop
	void Update( const double a_dt );
	// Draw a dynamically-sized generic grid 
	void DrawGrid( const unsigned int a_count );
	// Render things to screen
	void Render();
private:
};