#pragma once

#include "Engine\Core\GLApplication.h"

typedef unsigned int uint;

////
// Author: Jackson Luff
// Name: Tutorial02
// Type: class (standard)
// Parent: GLApplication
// Description:
// * In this Tutorial02 we learn about
// * OpenGL buffers, use of glProgram's
// * and working with basic shaders.
////
class Tutorial02 : public GLApplication
{
public:
	// Constructors
	Tutorial02();
	Tutorial02( const int a_width, const int a_height, const char* a_title );
	// Deconstructor
	~Tutorial02();

	// Initialise Things
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update( const double a_dt );
	// Render things to screen
	virtual void Render();
	
	// Draw a dynamically-sized generic grid 
	void GenerateModel( const char* a_folderPath, const char* a_filePath );
	// Function to create a grid
	void GenerateGrid( const unsigned int rows, const unsigned int cols );
private:
	// Our vertex and index buffers
	uint m_VAO;
	uint m_VBO;
	uint m_IBO;

	// Holds index count
	uint m_indexCount;
};