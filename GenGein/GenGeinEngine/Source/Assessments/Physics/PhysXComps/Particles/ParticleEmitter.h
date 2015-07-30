#pragma once
#include <PxPhysicsAPI.h>

class FBXModel;
using namespace physx;

//simple struct for our particles

struct Particle
{
	bool active;
	float maxTime;
};


//simple class for particle emitter.  For a real system we would make this a base class and derive different emitters from it by making functions virtual and overloading them.
class ParticleEmitter
{
public:
	ParticleEmitter(int _maxParticles, PxVec3 _position, PxParticleSystem* _ps, float _releaseDelay, FBXModel* a_model);
	~ParticleEmitter();
	
	void SetStartVelocityRange(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	
	void Update(const double delta);
	void ReleaseParticle(int);
	bool TooOld(int);
	void RenderParticles();

private:

	int GetNextFreeParticle();
	bool AddPhysXParticle(int particleIndex);

	int			m_maxParticles;
	Particle*	m_activeParticles;
	float		m_releaseDelay;
	int			m_numberActiveParticles;

	float		m_time;
	float		m_respawnTime;
	float		m_particleMaxAge;
	PxVec3		m_position;
	
	PxParticleSystem* m_ps;
	FBXModel* m_model;
	
	PxVec3 m_minVelocity;
	PxVec3 m_maxVelocity;
};