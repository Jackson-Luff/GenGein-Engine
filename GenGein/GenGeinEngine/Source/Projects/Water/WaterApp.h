#pragma once
#include "Engine\Core\BaseApp.h"

class FBXModel;
class FrameBuffObj;

class WaterApp : public BaseApp
{
public:
	WaterApp();
	WaterApp(const int a_width, const int a_hieght, const char* a_title);
	~WaterApp();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double& a_dt);
	// Render things to screen
	virtual void Render();
private:
	FBXModel* m_waterPlane;
	FrameBuffObj* m_reflectionFBO;
	FrameBuffObj* m_refractionFBO;
};