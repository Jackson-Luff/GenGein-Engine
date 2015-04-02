#pragma once
#include "Engine\Core\GLApplication.h"

class FBXModel;

////
// Author: Jackson Luff
// Name: Tutorial13
// Type: class (standard)
// Parent: GLApplication
// Description:
// * Physically based rendering
////
class Tutorial13 : public GLApplication
{
public:
	// Constructors
	Tutorial13();
	Tutorial13(c_int a_width, c_int a_height, const char* a_title);
	// Deconstructor
	~Tutorial13();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();
private:

	FBXModel* m_model;
};