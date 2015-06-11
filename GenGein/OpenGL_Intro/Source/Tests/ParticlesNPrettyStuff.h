#pragma once
#include "Engine\Core\BaseApp.h"

class GPUParticleEmitter;

class ParticlesNPrettyStuff : public BaseApp
{
public:
	ParticlesNPrettyStuff();
	ParticlesNPrettyStuff(c_int a_width, c_int a_height, c_pChar a_title);
	~ParticlesNPrettyStuff();

	// Initialised content 
	virtual void StartUp();
	// Deconstructed content
	virtual void ShutDown();
	// Update loop
	virtual void Update(const double_t a_dt);
	// Render to GUI
	virtual void Render();
private:
	GPUParticleEmitter* m_emitter;
};

