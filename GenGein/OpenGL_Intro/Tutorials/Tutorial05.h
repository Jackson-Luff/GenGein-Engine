#pragma once
#include "Engine\Core\GLApplication.h"\

class CPUParticleEmitter;
class Lighting;

////
// Author: Jackson Luff
// Name: Tutorial05
// Type: class (standard)
// Parent: GLApplication
// Description:
// * In Tutorial05 we create a basic
// * CPU particle system based on the emittion
// * of particle data and point manipulation
////
class Tutorial05 : public GLApplication
{
public:
	// Constructors
	Tutorial05();
	Tutorial05(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial05();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	glm::vec4 m_overallParticleColour;

	// Particle Emitter
	CPUParticleEmitter* m_emitter;
};