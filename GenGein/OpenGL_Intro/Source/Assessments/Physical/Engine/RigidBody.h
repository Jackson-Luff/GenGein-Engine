#pragma once
#include "Assets\PhysActor.h"

class RigidBody : public PhysActor
{
public:

	~RigidBody();
	RigidBody();
	RigidBody(const glm::vec3 a_pos, const glm::vec3 a_vel,
		const float a_rot, const float a_mass);

	virtual void Update(const glm::vec3 a_gravity, const float a_dt);
	virtual void Render();
	
	void ApplyForce(const glm::vec3 a_force);
	void ApplyForceToActor(RigidBody* const a_actorB, const glm::vec3 a_force);

	// Get Mass
	inline const float& GetMass() 
		{ return m_mass; }

	// Get Mass
	inline const float& Get2DRotation() 
		{ return m_rotation2D; }

	// Set | Get Position
	inline const glm::vec3 GetPosition() 
		{ return m_position; };
	inline void SetPosition(const glm::vec3& a_pos)
		{ m_position = a_pos; };

	// Set | Get Velocity
	inline const glm::vec3 GetVelocity() 
		{ return m_velocity; };
	inline void SetVelocity(const glm::vec3& a_vel)
		{ m_velocity = a_vel; };

	void ResetPosition();
protected:
	float m_rotation2D; // 2D so we only need a single float to represent our rot
	float m_mass;
	glm::vec3 m_position;
	glm::vec3 m_velocity;

private:
	glm::vec3 m_origin;
};