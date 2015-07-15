#pragma once
#include <vector>
#include <glm/glm.hpp>

class PhysActor;

class PhysScene
{
public:

	~PhysScene();
	PhysScene();
	PhysScene(const glm::vec3 a_gravity);

	void Reset();
	void Update(const double a_dt);
	void DebugScene();

	void AddActor(PhysActor*);
	void RemoveActor(PhysActor*);
	void RemoveAllActors();

	void AddGizmos(const glm::mat4& a_projView);

	void CheckForCollisions();
	
	// Plane Collision Checks
	static bool PlaneToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToAABB(PhysActor* a_A, PhysActor* a_B);
	
	// Sphere Collision Checks
	static bool SphereToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToAABB(PhysActor* a_A, PhysActor* a_B);
	
	// AABB Collision Checks
	static bool AABBToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool AABBToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool AABBToAABB(PhysActor* a_A, PhysActor* a_B);
	
	// Ray Collision Checks
	static bool RayToPlane(glm::vec3 a_point, PhysActor* a_B);
	static bool RayToSphere(glm::vec3 a_point, PhysActor* a_B);
	static bool RayToAABB(glm::vec3 a_point, PhysActor* a_B);

private:
	float m_timeStep;
	glm::vec3 m_gravity;
	std::vector<PhysActor*> m_actors;
};