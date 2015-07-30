#include "Engine\Objects\FBXModel.h"
#include "Assets\PhysActor.h"
#include "Assets\PhysPlane.h"
#include "Assets\PhysSphere.h"
#include "Assets\PhysAABB.h"
#include "Assets\PhysSpringJoint.h"
#include "PhysScene.h"

using BodyType = PhysActor::BodyType;
using ShapeType = PhysActor::ShapeType;

// typedef helper for collision array
typedef bool(*fn)(PhysActor*, PhysActor*);

// function pointer array for doing our collision
static fn collFuncArray[] =
{
	PhysScene::PlaneToPlane,	PhysScene::PlaneToSphere,	PhysScene::PlaneToAABB,		PhysScene::PlaneToJoint,
	PhysScene::SphereToPlane,	PhysScene::SphereToSphere,	PhysScene::SphereToAABB,	PhysScene::SphereToJoint,
	PhysScene::AABBToPlane,		PhysScene::AABBToSphere,	PhysScene::AABBToAABB,		PhysScene::AABBToJoint,
	PhysScene::JointToPlane,	PhysScene::JointToSphere,	PhysScene::JointToAABB,		PhysScene::JointToJoint,
};

PhysScene::PhysScene()
{}

PhysScene::~PhysScene()
{}

PhysScene::PhysScene(const glm::vec3 a_gravity) : PhysScene()
{
	m_gravity = a_gravity;

	m_sphereInst = new FBXModel();
	m_sphereInst->LoadFBX("Phys", "Data/Models/sphere.fbx", FBXFile::UNITS_CENTIMETER);

	m_planeInst = new FBXModel();
	m_planeInst->LoadFBX("Phys", "Data/Models/plane.fbx", FBXFile::UNITS_CENTIMETER);

	m_boxInst = new FBXModel();
	m_boxInst->LoadFBX("Phys", "Data/Models/cube.fbx", FBXFile::UNITS_CENTIMETER);
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
	// Update Actors
	for (auto& actor : m_actors)
	{
		if (actor->GetBodyID() == BodyType::STATIC)
			continue;

		actor->Update(m_gravity, a_dt);
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
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == a_actor)
		{
			switch (m_actors[i]->GetShapeID())
			{
			case ShapeType::PLANE:
				printf("Warning: Removed actor type Plane.\n");
			case ShapeType::SPHERE:
				printf("Warning: Removed actor type Sphere.\n");
			case ShapeType::AABB:
				printf("Warning: Removed actor type AABB.\n");
			case ShapeType::JOINT:
				printf("Warning: Removed actor type Joint.\n");
			}
			delete m_actors[i];
			m_actors[i] = NULL;
			m_actors.erase(m_actors.begin() + i);
			i--;
		}
	}
}

void PhysScene::RemoveAllActors()
{
	m_planeInst->Shutdown();
	m_planeInst = NULL;

	m_sphereInst->Shutdown();
	m_sphereInst = NULL;

	m_boxInst->Shutdown();
	m_boxInst = NULL;

	for (auto& it : m_actors)
	{
		delete it;
		it = nullptr;
	}

	m_actors.clear();
	printf("Warning: Removed all actors.\n");
}

void PhysScene::Render()
{
	for (auto& actor : m_actors)
	{
		if (actor->GetShapeID() == ShapeType::JOINT ||
			actor->GetShapeID() == ShapeType::NUMBER_OF_SHAPES)
			continue;

		if (actor->GetShapeID() == ShapeType::PLANE)
		{
			PhysPlane* plane = dynamic_cast<PhysPlane*>(actor);
			glm::vec3 centerPoint = plane->GetNormal() * plane->GetDistance();
			glm::mat4 t = glm::translate(centerPoint) * glm::scale(glm::vec3(100, 0.1, 100));
			m_planeInst->SetLocalTransform(t);
			m_planeInst->Render();
		}
		else if (actor->GetShapeID() == ShapeType::SPHERE)
		{
			PhysSphere* sphere = dynamic_cast<PhysSphere*>(actor);
			glm::mat4 t = glm::translate(sphere->GetPosition()) * glm::scale(glm::vec3(sphere->GetRadius()));
			m_sphereInst->SetLocalTransform(t);
			m_sphereInst->Render();
		}
		else if (actor->GetShapeID() == ShapeType::AABB)
		{
			PhysAABB* box = dynamic_cast<PhysAABB*>(actor);
			glm::mat4 t = glm::translate(box->GetPosition()) * glm::scale(box->GetSize());
			m_boxInst->SetLocalTransform(t);
			m_boxInst->Render();
		}
	}
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
		output = "PlaneToJoint";
		break;

	case 4:
		output = "SphereToPlane";
		break;
	case 5:
		output = "SphereToSphere";
		break;
	case 6:
		output = "SphereToAABB";
		break;
	case 7:
		output = "SphereToJoint";
		break;

	case 8:
		output = "AABBToPlane";
		break;
	case 9:
		output = "AABBToSphere";
		break;
	case 10:
		output = "AABBToAABB";
		break;
	case 11:
		output = "AABBToJoint";
		break;

	case 12:
		output = "JointToPlane";
		break;
	case 13:
		output = "JointToSphere";
		break;
	case 14:
		output = "JointToAABB";
		break;
	case 15:
		output = "JointToJoint";
		break;
	default:
		output = "WARNING: NO OUTPUT";
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

			int MaxShapeCount = (int)ShapeType::NUMBER_OF_SHAPES;

			int funcIndex = ((int)actorA->GetShapeID() * MaxShapeCount) + (int)actorB->GetShapeID();

			fn collFuncPtr = collFuncArray[funcIndex];
			if (collFuncPtr != NULL)
			{
				if (collFuncPtr(actorA, actorB))
					0;//OutputCollision(funcIndex);
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
	return SphereToPlane(a_B, a_A);
}

bool PhysScene::PlaneToAABB(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::PlaneToJoint(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

// Sphere Collision Checks

bool PhysScene::SphereToPlane(PhysActor* a_A, PhysActor* a_B)
{
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_A);
	PhysPlane* plane = dynamic_cast<PhysPlane*>(a_B);

	glm::vec3 planeNormal = plane->GetNormal();

	float dist = glm::dot(sphere->GetPosition(), planeNormal) - plane->GetDistance();

	// Check for interception
	float intersection = sphere->GetRadius() - dist;
	if (intersection > 0)
	{
		// Find the point where the collision occured (we need this for collision reponse later)
		// the plane is always static so collision response only applies to the sphere

		glm::vec3 forceVec = -1 * sphere->GetMass() * planeNormal *
			glm::dot(planeNormal, sphere->GetLinearVel());

		sphere->ApplyForce(forceVec * 2.0f);
		glm::vec3 newPos = sphere->GetPosition() + (planeNormal * intersection);
		sphere->SetPosition(newPos);
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
		glm::vec3 AVel = sphereA->GetLinearVel();
		glm::vec3 BVel = sphereB->GetLinearVel();
		glm::vec3 relVelocity = AVel - BVel;
		glm::vec3 collVec = collNormal * glm::dot(relVelocity, collNormal);
		glm::vec3 forceVec = collVec * 1.0f / (1 / sphereA->GetMass() + 1 / sphereB->GetMass());
		
		// Simple way to calculate the combined elasticity of the materials for restitution
		float combElasticity = (sphereA->GetElasticity() + sphereB->GetElasticity()) / 2.0f;
		
		BodyType sA_BodyID = sphereA->GetBodyID();
		BodyType sB_BodyID = sphereB->GetBodyID();

		// Use Newtons third law to apply collision forces to collided bodies.

		sphereA->ApplyForceToActor(sphereB, forceVec * (combElasticity));
		sphereA->ApplyForce(forceVec * -(combElasticity));

		// Move our spheres out of collision
		glm::vec3 sepVec = collNormal * intersection * 0.5f;

		if (sB_BodyID == BodyType::DYNAMIC)
			sphereB->SetPosition(sPosB + glm::vec3(glm::vec2(sepVec), 0));
		if (sA_BodyID == BodyType::DYNAMIC)
			sphereA->SetPosition(sPosA - glm::vec3(glm::vec2(sepVec), 0));

		return true;
	}

	return false;
}

bool PhysScene::SphereToAABB(PhysActor* a_A, PhysActor* a_B)
{
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_A);
	PhysAABB* box = dynamic_cast<PhysAABB*>(a_B);

	if (sphere == NULL || box == NULL)
		return false;

	glm::vec3 bPos = box->GetPosition();
	glm::vec3 bSize = box->GetSize();

	glm::vec3 sPos = sphere->GetPosition();
	float sRadius = sphere->GetRadius();

	glm::vec3 c1 = glm::vec3(bPos.x - (bSize.z / 2.f), bPos.y - (bSize.y / 2.f), bPos.z - (bSize.x / 2.f));
	glm::vec3 c2 = glm::vec3(bPos.x + (bSize.z / 2.f), bPos.y + (bSize.y / 2.f), bPos.z + (bSize.x / 2.f));

	float distSq = sRadius * sRadius;

	for (int i = 0; i < 3; i++)
	{
		if (sPos[i] < c1[i])
			distSq -= (sPos[i] - c1[i], 2.0f);
		else if (sPos[i] > c2[i])
			distSq -= pow(sPos[i] - c2[i], 2.0f);
	}

	if (distSq > 0)
	{
		return true;
	}

	return false;
}

bool PhysScene::SphereToJoint(PhysActor* a_A, PhysActor* a_B)
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
	PhysAABB* box =	   dynamic_cast<PhysAABB*>(a_A);
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_B);

	glm::vec3 bPos = box->GetPosition();
	glm::vec3 bSize = box->GetSize();
	
	glm::vec3 sPos = sphere->GetPosition();
	float sRadius = sphere->GetRadius();
	
	glm::vec3 c1 = glm::vec3(bPos.x - (bSize.z / 2.f), bPos.y - (bSize.y / 2.f), bPos.z - (bSize.x / 2.f));
	glm::vec3 c2 = glm::vec3(bPos.x + (bSize.z / 2.f), bPos.y + (bSize.y / 2.f), bPos.z + (bSize.x / 2.f));
	
	float distSq = sRadius * sRadius;
	
	for (int i = 0; i < 3; i++)
	{
		if (sPos[i] < c1[i])
			distSq -= (sPos[i] - c1[i], 2.0f);
		else if (sPos[i] > c2[i])
			distSq -= pow(sPos[i] - c2[i], 2.0f);
	}
	
	if (distSq > 0)
	{
		return true;
	}

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
	{
		
	}

	return false;
}

bool PhysScene::AABBToJoint(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

// Ray Collision Checks

bool PhysScene::JointToPlane(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::JointToSphere(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::JointToAABB(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

bool PhysScene::JointToJoint(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}