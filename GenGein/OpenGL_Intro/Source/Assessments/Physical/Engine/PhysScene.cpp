#include <AIE\Gizmos.h>

#include "Assets\PhysActor.h"
#include "Assets\PhysPlane.h"
#include "Assets\PhysSphere.h"
#include "Assets\PhysAABB.h"
#include "PhysScene.h"

// typedef helper for collision array
typedef bool(*fn)(PhysActor*, PhysActor*);

// function pointer array for doing our collision
static fn collFuncArray[] =
{
	PhysScene::PlaneToPlane,	PhysScene::PlaneToSphere,	PhysScene::PlaneToAABB,
	PhysScene::SphereToPlane,	PhysScene::SphereToSphere,	PhysScene::SphereToAABB,
	PhysScene::AABBToPlane,		PhysScene::AABBToSphere,	PhysScene::AABBToAABB, 
	//PhysScene::RayToPlane,		PhysScene::RayToSphere,		PhysScene::RayToAABB
};

PhysScene::PhysScene()
{
}

PhysScene::~PhysScene()
{
	Gizmos::destroy();
}

PhysScene::PhysScene(const glm::vec3 a_gravity)
{
	m_gravity = a_gravity;
	Gizmos::create();
}

void PhysScene::Reset()
{
	for (auto& actor : m_actors)
	{
		actor->ResetPosition();
	}
}

void PhysScene::Update(const double a_dt)
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
			case PhysActor::ShapeType::AABB:
				printf("Warning: Removed actor type AABB.\n");
			}
			
			m_actors.erase(m_actors.begin() + i);
			i--;
		}
	}
}

void PhysScene::RemoveAllActors()
{
	printf("Warning: Removed all actors.\n");
	m_actors.clear();
}

void OutputCollision(const int a_indexOfCollision)
{
	const char* output;

	switch (a_indexOfCollision)
	{
	case 0:
		output = "PlaneToPlane";
		break;
	case 1:
		output = "PlaneToSphere";
		break;
	case 2:
		output = "PlaneToAABB";
		break;
	case 3:
		output = "SphereToPlane";
		break;
	case 4:
		output = "SphereToSphere";
		break;
	case 5:
		output = "SphereToAABB";
		break;
	case 6:
		output = "AABBToPlane";
		break;
	case 7:
		output = "AABBToSphere";
		break;
	case 8:
		output = "AABBToAABB";
		break;
	default:
		break;
	}
	
	printf("Collision of %s!\n", output);

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
				if (collFuncPtr(actorA, actorB))
					OutputCollision(funcIndex);
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

bool PhysScene::PlaneToAABB(PhysActor* a_A, PhysActor* a_B)
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

	glm::vec3 sPosA = sphereA->GetPosition();
	glm::vec3 sPosB = sphereB->GetPosition();

	glm::vec3 delta = sPosB - sPosA;
	
	float dist = glm::length(delta);
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
		sphereA->SetPosition(sPosA - glm::vec3(glm::vec2(sepVec), 0));
		sphereB->SetPosition(sPosB + glm::vec3(glm::vec2(sepVec), 0));

		printf("Two spheres have collided!\n");
		return true;
	}

	return false;
}

bool PhysScene::SphereToAABB(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

// AABB Collision Checks

bool PhysScene::AABBToPlane(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::AABBToSphere(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::AABBToAABB(PhysActor* a_A, PhysActor* a_B)
{
	PhysAABB* AABB_A = dynamic_cast<PhysAABB*>(a_A);
	PhysAABB* AABB_B = dynamic_cast<PhysAABB*>(a_B);

	glm::vec3 A_min = AABB_A->GetPosition() - AABB_A->GetSize() / 2.0f;
	glm::vec3 A_max = AABB_A->GetPosition() + AABB_A->GetSize() / 2.0f;

	glm::vec3 B_min = AABB_B->GetPosition() - AABB_B->GetSize() / 2.0f;
	glm::vec3 B_max = AABB_B->GetPosition() + AABB_B->GetSize() / 2.0f;

	// Use Newtons third law to apply collision forces to collided bodies.
	glm::vec3 delta = AABB_B->GetPosition() - AABB_A->GetPosition();

	// Check if AABB_A's max is greater than AABB_B's min and
	// AABB_A's min is less than AABB_B's max
	if ((A_max.x > B_min.x &&
		A_min.x < B_max.x &&
		A_max.y > B_min.y &&
		A_min.y < B_max.y &&
		A_max.z > B_min.z &&
		A_min.z < B_max.z))
	{}

	return false;
}

// Ray Collision Checks

bool PhysScene::RayToPlane(glm::vec3 a_point, PhysActor* a_B)
{

	return false;
}

bool PhysScene::RayToSphere(glm::vec3 a_point, PhysActor* a_B)
{
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_B);

	//Calculate the squared distance from the point to the center of the sphere
	float dist = glm::length(sphere->GetPosition() - a_point);
	float distSq = glm::pow(dist, 2);
	float radiSq = glm::pow(sphere->GetRadius(), 2);

	//Calculate if the squared distance between the sphere's center and the point
	//is less than the squared radius of the sphere
	if (distSq < radiSq)
		return true;

	return false;
}

bool PhysScene::RayToAABB(glm::vec3 a_point, PhysActor* a_B)
{
	PhysAABB* AABB = dynamic_cast<PhysAABB*>(a_B);

	glm::vec3 min = AABB->GetPosition() - AABB->GetSize() / 2.0f;
	glm::vec3 max = AABB->GetPosition() + AABB->GetSize() / 2.0f;

	if (a_point.x > min.x && a_point.x < max.x &&
		a_point.y > min.y && a_point.y < max.y &&
		a_point.z > min.z && a_point.z < max.z)
	{
		return true;
	}

	//If not, then return false
	return false;
}

// Private Functions:

void PhysScene::AddGizmos(const glm::mat4& a_projView)
{
	Gizmos::clear();
	for (auto& actor : m_actors)
	{
		actor->Render();
	}

	Gizmos::draw(a_projView);
}