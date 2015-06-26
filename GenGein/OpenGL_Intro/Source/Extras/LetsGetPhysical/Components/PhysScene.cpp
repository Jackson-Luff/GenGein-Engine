#include "Assets\PhysActor.h"
#include "Assets\PhysPlane.h"
#include "Assets\PhysSphere.h"
#include "Assets\PhysCube.h"
#include "PhysScene.h"

// typedef helper for collision array
typedef bool(*fn)(PhysActor*, PhysActor*);

// function pointer array for doing our collision
static fn collFuncArray[] =
{
	PhysScene::PlaneToPlane,	PhysScene::PlaneToSphere,	PhysScene::PlaneToCube,
	PhysScene::SphereToPlane,	PhysScene::SphereToSphere,	PhysScene::SphereToCube,
	PhysScene::CubeToPlane,		PhysScene::CubeToSphere,	PhysScene::CubeToCube
};

PhysScene::PhysScene()
{
}

PhysScene::~PhysScene()
{
}

PhysScene::PhysScene(const glm::vec3 a_gravity)
{
	m_gravity = a_gravity;
}

void PhysScene::Reset()
{
	for (auto& actor : m_actors)
	{
		actor->ResetPosition();
	}
}

void PhysScene::Update(const float a_dt)
{
	// Timestep update
	m_timeStep = 0.0001f;
	// Update Actors
	for (auto& actor : m_actors)
	{
		actor->Update(m_gravity, m_timeStep);
	}
	// Update Collision Check
	CheckForCollisions();
}

void PhysScene::DebugScene()
{}

void PhysScene::AddActor(PhysActor* a_actor)
{
	if (std::find(m_actors.begin(), m_actors.end(), a_actor) == m_actors.end())
	{
		printf("Warning: Adding actor\n");
		m_actors.push_back(a_actor);
	}
	else
		printf("Warning: Actor already exists.\n");
}

void PhysScene::RemoveActor(PhysActor* a_actor)
{
	
	for (uint32_t i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == a_actor)
		{
			switch (m_actors[i]->GetID())
			{
			case PhysActor::ShapeType::PLANE:
				printf("Warning: Removed actor type plane.\n");
			case PhysActor::ShapeType::SPHERE:
				printf("Warning: Removed actor type sphere.\n");
			case PhysActor::ShapeType::CUBE:
				printf("Warning: Removed actor type cube.\n");
			}
			
			m_actors.erase(m_actors.begin() + i);
			i--;
		}
	}
}

void PhysScene::RemoveAllActors()
{
	printf("Warning: Removed all actors.\n");
	for (uint32_t i = 0; i < m_actors.size(); i++)
	{
		m_actors.erase(m_actors.begin() + i);
		i--;
	}
}

void PhysScene::CheckForCollisions()
{
	int actCount = m_actors.size();
	for (int o = 0; o < actCount - 1; o++)
	{
		for (int i = o+1; i < actCount; i++)
		{
			PhysActor* actorA = m_actors[o];
			PhysActor* actorB = m_actors[i];
			int MaxShapeCount = (int)PhysActor::ShapeType::NUMBER_OF_SHAPES;

			int funcIndex = ((int)actorA->GetID() * MaxShapeCount) + (int)actorB->GetID();

			fn collFuncPtr = collFuncArray[funcIndex];
			if (collFuncPtr != NULL)
			{
				collFuncPtr(actorA, actorB);
			}
		}
	}
}

// Plane Collision Checks
bool PhysScene::PlaneToPlane(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::PlaneToSphere(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::PlaneToCube(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

// Sphere Collision Checks
bool PhysScene::SphereToPlane(PhysActor* a_A, PhysActor* a_B)
{
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_A);
	PhysPlane* plane = dynamic_cast<PhysPlane*>(a_B);

	glm::vec3 collNormal = plane->GetNormal();

	float dist = glm::dot(sphere->GetPosition(), plane->GetNormal()) - plane->GetDistance();

	// If we're behind plane then we flip the normal
	if (dist < 0)
	{
		collNormal *= -1;
		dist *= -1;
	}

	// Check for interception
	float intersection = sphere->GetRadius() - dist;
	if (intersection > 0)
	{
		// Find the point where the collision occured (we need this for collision reponse later)
		// the plane is always static so collision response only applies to the sphere

		glm::vec3 planeNormal = plane->GetNormal();
		if (dist < 0)
		{
			// flip the normal if we are behind the plane
			planeNormal *= -1;
		}

		glm::vec3 forceVec = -1 * sphere->GetMass() * planeNormal *
			glm::dot(planeNormal, sphere->GetVelocity());

		sphere->ApplyForce(forceVec * 2.0f);
		//sphere->SetPosition(sphere->GetPosition() * collNormal * intersection * 0.5f);
		return true;
	}
		
	return false;
}

bool PhysScene::SphereToSphere(PhysActor* a_A, PhysActor* a_B)
{
	PhysSphere* sphereA = dynamic_cast<PhysSphere*>(a_A);
	PhysSphere* sphereB = dynamic_cast<PhysSphere*>(a_B);

	if (sphereA == NULL || sphereB == NULL)
		return false;

	glm::vec3 delta = sphereB->GetPosition() - sphereA->GetPosition();
	float dist = glm::length(sphereB->GetPosition() - sphereA->GetPosition());
	float radii = sphereA->GetRadius() + sphereB->GetRadius();
	float intersection = radii - dist;
	if (intersection > 0)
	{
		glm::vec3 collNormal = glm::normalize(delta);
		glm::vec3 relVelocity = sphereA->GetVelocity() - sphereB->GetVelocity();
		glm::vec3 collVec = collNormal * glm::dot(relVelocity, collNormal);
		glm::vec3 forceVec = collVec * 1.0f / (1 / sphereA->GetMass() + 1 / sphereB->GetMass());
		// Use Newtons third law to apply collision forces to collided bodies.
		sphereA->ApplyForceToActor(sphereB, forceVec * 2.0f);
		sphereA->ApplyForce(-forceVec * 2.0f);

		// Move our spheres out of collision
		glm::vec3 sepVec = collNormal * intersection * 0.5f;
		sphereA->SetPosition(sphereA->GetPosition() - glm::vec3(glm::vec2(sepVec), 0));
		sphereB->SetPosition(sphereB->GetPosition() + glm::vec3(glm::vec2(sepVec), 0));

		printf("Two spheres have collided!\n");
		return true;
	}

	return false;
}

bool PhysScene::SphereToCube(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

// Cube Collision Checks
bool PhysScene::CubeToPlane(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::CubeToSphere(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::CubeToCube(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}
// Private Functions:

void PhysScene::AddGizmos()
{
	for (auto& actor : m_actors)
	{
		actor->MakeGizmo();
	}
}