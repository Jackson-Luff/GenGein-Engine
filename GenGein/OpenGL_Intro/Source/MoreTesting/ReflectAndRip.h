#pragma once
#include "Engine\Core\GLApplication.h"

class FBXModel;
class TransGrid;

class ReflectAndRip : public GLApplication
{
public:
	ReflectAndRip();
	ReflectAndRip(c_int a_width, c_int a_height, c_pChar a_title);
	~ReflectAndRip();

	// Initialised content 
	virtual void StartUp();
	// Deconstructed content
	virtual void ShutDown();
	// Update loop
	virtual void Update(const double a_dt);
	// Render to GUI
	virtual void Render();

	void CreateRenderObject(); 
	void SetUpTextureData();
	void SetUpDepthBuffer();

private:
	FBXModel* m_sphere;
	TransGrid* m_waterPlane;
	
	// FBO stuff
	uint m_fbo;
};

