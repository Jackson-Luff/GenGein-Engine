#pragma once
#include "PhysActor.h"

class RigidBody;

class PhysSpringJoint : public PhysActor
{
public:
	PhysSpringJoint(RigidBody* a_connA, RigidBody* a_connB,
		float a_springCoefficient, float a_damping,
		BodyType a_bodyID);

	~PhysSpringJoint();

	virtual void Update(const glm::vec3 a_gravity, const double a_dt);

private:
	RigidBody* m_conns[2];
	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};