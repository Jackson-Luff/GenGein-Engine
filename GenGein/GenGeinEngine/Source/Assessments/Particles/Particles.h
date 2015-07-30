#pragma once
#include "Engine\Core\BaseApp.h"

class GPUParticleEmitter;

class Particles : public BaseApp
{
public:
	Particles();
	Particles(const int a_width, const int a_hieght, const char* a_title);
	~Particles();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double& a_dt);
	// Render things to screen
	virtual void Render();
private:
	GPUParticleEmitter* m_emitter;
};