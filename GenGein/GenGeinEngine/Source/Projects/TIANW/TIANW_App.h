#pragma once
#include "Engine\Core\BaseApp.h"

class Terrain;
class WaterPlane;
class Skybox;
class FBXModel;

////
// Author: Jackson Luff
// Name: TIANW
// Type: class (standard)
// Parent: none
// Description:
// * 'Tomorrow is a new world' is a project
// * I put upon myself to freshen and improve
// * my graphics programming.
// * I was feeling a bit rusty but hopefully
// * this says otherwise.
// * 
// * Features:
// * 
// * Future-Features:
// * Visually pleasing.
// * Generally pretty optimised.
// * No memory leaks.
// * Procedurally Generated Terrain / Water.
// * correct foliage application.
// * 
////
class TIANW_App : public BaseApp
{
public:
	TIANW_App();
	TIANW_App(const int a_width, const int a_hieght, const char* a_title);
	~TIANW_App();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(double a_dt);
	// Render things to screen
	virtual void Render();
private:
	// SkyBox 
	SkyBox* m_pSkyBox;
	// Foliage
	// Environment
	Terrain* m_pTerrain;
	// Water
	WaterPlane* m_pWater;
	// Sun
	FBXModel* m_pSunModel;
	FBXModel* m_pPlane;
	FBXModel* m_pSphere;
	FBXModel* m_pCube;
};