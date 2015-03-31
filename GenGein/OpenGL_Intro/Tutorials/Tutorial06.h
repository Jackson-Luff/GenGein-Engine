#pragma once
#include <vector>
#include "Engine\Core\GLApplication.h"
class FBXModel;
class GPUParticleEmitter;

////
// Author: Jackson Luff
// Name: Tutorial06
// Type: class (standard)
// Parent: GLApplication
// Description:
// * In Tutorial06 we create an advanced 
// * GPU particle system based on the emittion
// * of particle data and point manipulation
////
class Tutorial06 : public GLApplication
{
public:
	// Constructors
	Tutorial06();
	Tutorial06(const int a_width, const int a_height, const char* a_title);
	// Deconstructor
	~Tutorial06();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double a_dt);
	// Render things to screen
	virtual void Render();

private:
	//Modified starter/end colours
	glm::vec4 m_starterColour;
	glm::vec4 m_endingColour;

	//FBX Model
	FBXModel* m_pModel;
	// Particle Emitter
	GPUParticleEmitter* m_pEmitter;

	std::vector<glm::vec3> m_particleSpawn;

	// fake position
	glm::vec3 spherePos;
};