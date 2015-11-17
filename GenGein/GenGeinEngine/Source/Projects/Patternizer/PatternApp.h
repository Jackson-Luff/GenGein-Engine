#pragma once
#include "Engine\Core\BaseApp.h"

class FBXModel;
class FrameBuffObj;

class PatternApp : public BaseApp
{
public:
	PatternApp();
	PatternApp(const int a_width, const int a_hieght, const char* a_title);
	~PatternApp();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double& a_dt);
	// Render things to screen
	virtual void Render();
private:
	FBXModel* m_patternPlane;
	FBXModel* m_displayPlane;
	FrameBuffObj* m_postProcessEffect;
};