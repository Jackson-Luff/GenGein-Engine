#pragma once
#include "Engine\Core\GLApplication.h"\

class FBXModel;
class Lighting;

////
// Author: Jackson Luff
// Name: Tutorial04
// Type: class (standard)
// Parent: GLApplication
// Description:
// * In Tutorial04 we load in and animate
// * and texture an FBXMesh via loading in
// * an FBX file
////
class Tutorial04 : public GLApplication
{
public:
	// Constructors
	Tutorial04();
	Tutorial04(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial04();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:

	// OBJ
	FBXModel* m_pFbxModel;
	// Lights
	Lighting* m_ambientLight;
	Lighting* m_pointLight;
};