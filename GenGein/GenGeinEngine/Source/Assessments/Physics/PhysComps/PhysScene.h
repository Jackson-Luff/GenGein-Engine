#pragma once
#include <vector>
#include <glm/glm.hpp>

class PhysActor;
class FBXModel;

class PhysScene
{
public:

	~PhysScene();
	PhysScene();
	PhysScene(const glm::vec3 a_gravity);

	void Reset();
	void Update(const double a_dt);
	void DebugScene();

	void AddActor(PhysActor* a_actor);
	void RemoveActor(PhysActor* a_actor);
	void RemoveAllActors();

	void Render();

	void CheckForCollisions();
	
	// Plane Collision Checks
	static bool PlaneToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToAABB(PhysActor* a_A, PhysActor* a_B);
	static bool PlaneToJoint(PhysActor* a_A, PhysActor* a_B);

	// Sphere Collision Checks
	static bool SphereToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToAABB(PhysActor* a_A, PhysActor* a_B);
	static bool SphereToJoint(PhysActor* a_A, PhysActor* a_B);

	// AABB Collision Checks
	static bool AABBToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool AABBToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool AABBToAABB(PhysActor* a_A, PhysActor* a_B);
	static bool AABBToJoint(PhysActor* a_A, PhysActor* a_B);

	// Ray Collision Checks
	static bool JointToPlane(PhysActor* a_A, PhysActor* a_B);
	static bool JointToSphere(PhysActor* a_A, PhysActor* a_B);
	static bool JointToAABB(PhysActor* a_A,PhysActor* a_B);
	static bool JointToJoint(PhysActor* a_A, PhysActor* a_B);

private:
	glm::vec3 m_gravity;
	std::vector<PhysActor*> m_actors;

	// Instances of re-used models:
	FBXModel* m_planeInst; 
	FBXModel* m_sphereInst;
	FBXModel* m_boxInst;
};