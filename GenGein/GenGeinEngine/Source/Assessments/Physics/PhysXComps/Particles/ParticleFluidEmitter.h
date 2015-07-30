#pragma once
#include <PxPhysicsAPI.h>
#include <glm\ext.hpp>

class FBXModel;
using namespace physx;

//simple struct for our particles

struct FluidParticle
{
	bool active;
	float maxTime;
};


//simple class for particle emitter.  For a real system we would make this a base class and derive different emitters from it by making functions virtual and overloading them.
class ParticleFluidEmitter
{
	
public:
	ParticleFluidEmitter(int _maxParticles, PxVec3 _position, PxParticleFluid* _pf, float _releaseDelay, FBXModel* a_model);
	~ParticleFluidEmitter();
	void Update(const double delta);
	void ReleaseParticle(int);
	bool TooOld(int);
	void RenderParticles();

	void SetStartVelocityRange(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
private:
	int GetNextFreeParticle();
	bool AddPhysXParticle(int particleIndex);

	PxParticleFluid*	m_pf;
	int					m_rows;
	int					m_cols;
	int					m_depth;

	int					m_maxParticles;
	FluidParticle*		m_activeParticles;
	float				m_releaseDelay;
	int					m_numberActiveParticles;

	float				m_time;
	float				m_respawnTime;
	float				m_particleMaxAge;
	PxVec3				m_position;
	int					m_boxWidth;
	int					m_boxHeight;

	PxVec3				m_minVelocity;
	PxVec3				m_maxVelocity;

	FBXModel* m_model;
};