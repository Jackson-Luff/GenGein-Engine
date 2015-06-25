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
	void Update(const float a_dt);
	void DebugScene();

	void AddActor(PhysActor*);
	void RemoveActor(PhysActor*);
	void RemoveAllActors();

	void AddGizmos();

	void CheckForCollisions();
	
	// Plane Collision Checks
	static bool PlaneToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToCube(PhysActor* a_A, PhysActor* a_B);
	// Sphere Collision Checks
	static bool SphereToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToCube(PhysActor* a_A, PhysActor* a_B);
	// Cube Collision Checks
	static bool CubeToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool CubeToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool CubeToCube(PhysActor* a_A, PhysActor* a_B);

private:
	float m_timeStep;
	glm::vec3 m_gravity;
	std::vector<PhysActor*> m_actors;
};