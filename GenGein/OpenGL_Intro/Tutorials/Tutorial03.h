#pragma once
#include "Engine\Core\GLApplication.h"

class ObjMesh;
class Lighting;

////
// Author: Jackson Luff
// Name: Tutorial03
// Type: class (standard)
// Parent: GLApplication
// Description:
// * In this Tutorial03 we learn
// * loading in an OBJ + advanced texturing. 
////
class Tutorial03 : public GLApplication
{
public:
	// Constructors
	Tutorial03();
	Tutorial03(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial03();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update( const double a_dt );
	// Render things to screen
	virtual void Render();

private:
	// Vertex Array Object
	unsigned int m_VAO;

	// OBJ
	ObjMesh* m_pObj;
	// Lights
	Lighting* m_ambientLight;
	Lighting* m_pointLight;
};