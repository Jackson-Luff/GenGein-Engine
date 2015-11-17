#pragma once
#include "PhysActor.h"

class RigidBody : public PhysActor
{
public:

	~RigidBody();
	RigidBody();
	RigidBody(const glm::vec3 a_pos, const float a_mass);

	virtual void Update(const glm::vec3 a_gravity, const double a_dt);
	
	void ApplyForce(const glm::vec3 a_force);
	void ApplyForceToActor(RigidBody* const a_actorB, const glm::vec3 a_force);
	void ApplyTorque(float a_torque, glm::vec3 a_dir);

	virtual void ResetPosition();

	// Get | Set Mass
	inline const float& GetMass() 
		{ return m_mass; }
	inline void SetMass(const float& a_mass)
		{ m_mass = a_mass; };

	// Get | Set Elasticity
	inline const float& GetElasticity()
		{ return m_elasticity; }
	inline void SetElasticity(const float& a_elasticity)
		{ m_elasticity = a_elasticity; };

	// Get | Set Linear Velocity
	inline const float GetLinearDrag() 
		{ return m_linearDrag; };
	inline void SetLinearDrag(const float& a_drag)
		{ m_linearDrag = a_drag; };

	// Get | Set Linear Velocity
	inline const float GetAngularDrag() 
		{ return m_angularDrag; };
	inline void SetAngularDrag(const float& a_drag)
		{ m_angularDrag = a_drag; };

	// Get | Set Position
	inline glm::vec3& GetRotation()
		{ return m_rotation; };
	inline void SetRotation(const glm::vec3& a_rot)
		{ m_rotation = a_rot; };

	// Get | Set Position
	inline const glm::vec3 GetPosition() 
		{ return m_position; };
	inline void SetPosition(const glm::vec3& a_pos)
		{ m_position = a_pos; };

	// Get | Set Linear Velocity
	inline const glm::vec3& GetLinearVel()
		{ return m_linearVelocity; };
	inline void SetLinearVel(const glm::vec3& a_vel)
		{ m_linearVelocity = a_vel; };

	// Get | Set Angular Velocity
	inline const glm::vec3& GetAngularVel() 
		{ return m_angularVelocity; };
	inline void SetAngularVel(const glm::vec3& a_vel)
		{ m_angularVelocity = a_vel; };

protected:
	float m_mass;
	float m_elasticity;
	float m_linearDrag;
	float m_angularDrag;
	glm::vec3 m_rotation;
	glm::vec3 m_position;
	glm::vec3 m_linearVelocity;
	glm::vec3 m_angularVelocity;
private:
	glm::vec3 m_origin;
};