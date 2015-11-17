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
	for (unsigned int o = 0; o < m_actors.size() - 1; o++)
	{
		for (unsigned int i = 0; i < m_actors.size() - 1; i++)
		{
			if (o == i)
				continue;

			if (m_actors[o]->GetBodyID() == BodyType::STATIC)
				continue;

			if (m_actors[o]->GetShapeID() != ShapeType::SPHERE &&
				m_actors[o]->GetShapeID() != ShapeType::AABB)
				continue;

			m_actors[o]->ResetPosition();

			float disp = glm::length(m_actors[i]->GetPosition() - m_actors[o]->GetPosition());

			if (disp < 0.2f)
			{
				m_actors.erase(m_actors.begin() + i);
				i--;
				o--;
			}
		}
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
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->GetShapeID() == ShapeType::JOINT ||
			m_actors[i]->GetShapeID() == ShapeType::NUMBER_OF_SHAPES)
			continue;

		if (m_actors[i]->GetShapeID() == ShapeType::PLANE)
		{
			PhysPlane* plane = dynamic_cast<PhysPlane*>(m_actors[i]);
			glm::mat4 t = glm::translate(glm::vec3(-55.0f, 0, 0)) * glm::scale(glm::vec3(2.5f, 1, 2.5f));
			m_planeInst->SetLocalTransform(t);
			m_planeInst->Render();
		}
		else if (m_actors[i]->GetShapeID() == ShapeType::SPHERE)
		{
			PhysSphere* sphere = dynamic_cast<PhysSphere*>(m_actors[i]);

			glm::mat4 r = glm::rotate(sphere->GetRotation().x, glm::vec3(1,0,0));
			r *= glm::rotate(sphere->GetRotation().y, glm::vec3(0,1,0));
			r *= glm::rotate(sphere->GetRotation().z, glm::vec3(0,0,1));

			glm::mat4 t = glm::translate(sphere->GetPosition()) * glm::scale(glm::vec3(sphere->GetRadius()));
			m_sphereInst->SetLocalTransform(t);
			m_sphereInst->Render();
		}
		else if (m_actors[i]->GetShapeID() == ShapeType::AABB)
		{
			PhysAABB* box = dynamic_cast<PhysAABB*>(m_actors[i]);
			glm::mat4 t = glm::translate(box->GetPosition()) * glm::scale(box->GetSize());
			m_boxInst->SetLocalTransform(t);
			m_boxInst->Render();
		}
		
		float dist = length(abs(m_actors[i]->GetPosition()));
		if(dist > 130)
		{
 			m_actors.erase(m_actors.begin() + i);
		}
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

			if (actorA->GetBodyID() == BodyType::STATIC &&
				actorB->GetBodyID() == BodyType::STATIC)
				continue;

			int MaxShapeCount = (int)ShapeType::NUMBER_OF_SHAPES;

			int funcIndex = ((int)actorA->GetShapeID() * MaxShapeCount) + (int)actorB->GetShapeID();

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
	PhysPlane* plane = dynamic_cast<PhysPlane*>(a_A);
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_B);

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

bool PhysScene::PlaneToAABB(PhysActor* a_A, PhysActor* a_B)
{
	PhysPlane* plane = dynamic_cast<PhysPlane*>(a_A);
	PhysAABB*  box	 = dynamic_cast<PhysAABB* >(a_B);

	glm::vec3 planePos	= plane->GetPosition();
	glm::vec3 planeNorm = plane->GetNormal();
	float planeDist		= plane->GetDistance();

	glm::vec3 boxPos = box->GetPosition();
	glm::vec3 hBSize = box->GetSize();

	glm::vec3 corners[8]
	{
		// Bottom -> Winding [3] [0] [1] [2]
		{ boxPos + -hBSize }, // Left Backward
		{ boxPos + glm::vec3(-hBSize.x, -hBSize.y, +hBSize.z) }, // Left Forward
		{ boxPos + glm::vec3(+hBSize.x, -hBSize.y, +hBSize.z) }, // Right Forward
		{ boxPos + glm::vec3(+hBSize.x, -hBSize.y, -hBSize.z) }, // Right Backward

		// Top -> Winding = [1] [2] [3] [0] 
		{ boxPos + hBSize }, // Right Forward
		{ boxPos + glm::vec3(+hBSize.x, +hBSize.y, -hBSize.z) }, // Right Backward
		{ boxPos + glm::vec3(-hBSize.x, +hBSize.y, -hBSize.z) }, // Left Backward
		{ boxPos + glm::vec3(-hBSize.x, +hBSize.y, +hBSize.z) }, // Left Forward
	};

	float closest = glm::dot(boxPos, planeNorm);
	for (auto& point : corners)
	{
		if (glm::dot(point, planeNorm) < closest)
			closest = glm::dot(point, planeNorm);

		if (closest < planeDist)
		{
			float intersection = planeDist - closest;
			glm::vec3 force = -1 * box->GetMass() * planeNorm * glm::dot(planeNorm, box->GetLinearVel());
			box->ApplyForce(2 * force);
			box->SetPosition(box->GetPosition() + (planeNorm * intersection));
			return true;
		}
	}

	return false;
}

bool PhysScene::PlaneToJoint(PhysActor* a_A, PhysActor* a_B)
{
	return false;
}

// Sphere Collision Checks

bool PhysScene::SphereToPlane(PhysActor* a_A, PhysActor* a_B)
{
	return PlaneToSphere(a_B, a_A);
}

bool PhysScene::SphereToAABB(PhysActor* a_A, PhysActor* a_B)
{
	PhysSphere* sphere = dynamic_cast<PhysSphere*>(a_A);
	PhysAABB* box = dynamic_cast<PhysAABB*>(a_B);

	glm::vec3 spherePos	= sphere->GetPosition();
	float sphereRadius	= sphere->GetRadius();
	float sqrRadii = powf(sphere->GetRadius(), 2.0f);
	
	glm::vec3 boxPos  = box->GetPosition();
	glm::vec3 hBoxSize = box->GetSize();

	glm::vec3 boxMin = boxPos + -hBoxSize;
	glm::vec3 boxMax = boxPos +  hBoxSize;

	glm::vec3 inter = glm::vec3(-1);
	
	for (int i = 0; i < 3; i++)
	{
		if (spherePos[i] < boxMin[i])
			inter[i] = powf(spherePos[i] - boxMin[i], 2.0f);
		else if (spherePos[i] > boxMax[i])
			inter[i] = powf(spherePos[i] - boxMax[i], 2.0f);

		sqrRadii -= inter[i];
	}

	if (sqrRadii > 0)
	{
		glm::vec3 sphereMin = spherePos - glm::vec3(sphereRadius);
		glm::vec3 sphereMax = spherePos + glm::vec3(sphereRadius);

		for (int i = 0; i < 3; i++)
			inter[i] = glm::min(boxMax[i] - sphereMin[i], sphereMax[i] - boxMin[i]);

		float minInter = glm::min(glm::min(inter.x, inter.y), inter.z);

		glm::vec3 collisionNorm;

		if (minInter == inter[0])
		{
			collisionNorm = glm::vec3(1, 0, 0);
			if (boxPos[0] < spherePos[0])
			{
				if (box->GetBodyID() == BodyType::DYNAMIC)
					box->SetPosition(glm::vec3(boxPos.x - minInter / 2.0f, boxPos.y, boxPos.z));
				if (sphere->GetBodyID() == BodyType::DYNAMIC)
					sphere->SetPosition(glm::vec3(spherePos.x + minInter / 2.0f, spherePos.y, spherePos.z));
			}
			else
			{
				collisionNorm *= -1.0f;
				if (box->GetBodyID() == BodyType::DYNAMIC)
					box->SetPosition(glm::vec3(boxPos.x + minInter / 2.0f, boxPos.y, boxPos.z));
				if (sphere->GetBodyID() == BodyType::DYNAMIC)
					sphere->SetPosition(glm::vec3(spherePos.x - minInter / 2.0f, spherePos.y, spherePos.z));
			}
		}
		else if (minInter == inter[1])
		{
			collisionNorm = glm::vec3(0, 1, 0);
			if (boxPos[1] < spherePos[1])
			{
				if (box->GetBodyID() == BodyType::DYNAMIC)
					box->SetPosition(glm::vec3(boxPos.x, boxPos.y - minInter / 2.0f, boxPos.z));
				if (sphere->GetBodyID() == BodyType::DYNAMIC)
					sphere->SetPosition(glm::vec3(spherePos.x, spherePos.y + minInter / 2.0f, spherePos.z));
			}
			else
			{
				collisionNorm *= -1.0f;
				if (box->GetBodyID() == BodyType::DYNAMIC)
					box->SetPosition(glm::vec3(boxPos.x, boxPos.y + minInter / 2.0f, boxPos.z));
				if (sphere->GetBodyID() == BodyType::DYNAMIC)
					sphere->SetPosition(glm::vec3(spherePos.x, spherePos.y - minInter / 2.0f, spherePos.z));
			}
		}
		else if (minInter == inter[2])
		{
			collisionNorm = glm::vec3(0, 0, 1);
			if (boxPos[2] < spherePos[2])
			{
				if (box->GetBodyID() == BodyType::DYNAMIC)
					box->SetPosition(glm::vec3(boxPos.x, boxPos.y, boxPos.z - minInter / 2.0f));
				if (sphere->GetBodyID() == BodyType::DYNAMIC)
					sphere->SetPosition(glm::vec3(spherePos.x, spherePos.y, spherePos.z + minInter / 2.0f));
			}
			else
			{
				collisionNorm *= -1.0f;
				if (box->GetBodyID() == BodyType::DYNAMIC)
					box->SetPosition(glm::vec3(boxPos.x, boxPos.y, boxPos.z + minInter / 2.0f));
				if (sphere->GetBodyID() == BodyType::DYNAMIC)
					sphere->SetPosition(glm::vec3(spherePos.x, spherePos.y, spherePos.z - minInter / 2.0f));
			}
		}

		glm::vec3 relVel = sphere->GetLinearVel() - box->GetLinearVel();
		glm::vec3 colVec = collisionNorm * glm::abs(glm::dot(relVel, collisionNorm));
		glm::vec3 forceVec = colVec * 1.0f / (1 / sphere->GetMass() + 1 / box->GetMass());
		float combElasticity = (sphere->GetElasticity() + box->GetElasticity()) / 2.0f;

		if (sphere->GetBodyID() == BodyType::DYNAMIC)
			sphere->ApplyForce(forceVec * combElasticity);
		else if (box->GetBodyID() == BodyType::DYNAMIC)
			box->ApplyForce(-forceVec * combElasticity);
		
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
	float sARadius = sphereA->GetRadius();
	float sBRadius = sphereB->GetRadius();
	float radii = sARadius + sBRadius;
	float intersection = radii - dist;

	if (intersection > 0)
	{
		glm::vec3 collNormal = glm::normalize(delta);
		glm::vec3 AVel = sphereA->GetLinearVel();
		glm::vec3 BVel = sphereB->GetLinearVel();
		glm::vec3 relVelocity = AVel - BVel;
		glm::vec3 collVec = collNormal * glm::dot(relVelocity, collNormal);
		float combineMass = 1.0f / (1 / sphereA->GetMass() + 1 / sphereB->GetMass());
		glm::vec3 forceVec = collVec * combineMass;
	
		// Simple way to calculate the combined elasticity of the materials for restitution
		float combElasticity = (sphereA->GetElasticity() + sphereB->GetElasticity()) / 2.0f;
	
		BodyType sA_BodyID = sphereA->GetBodyID();
		BodyType sB_BodyID = sphereB->GetBodyID();
	
		// Use Newtons third law to apply collision forces to collided bodies.
	
		if (sB_BodyID == BodyType::DYNAMIC)
			sphereA->ApplyForceToActor(sphereB, forceVec * (combElasticity));
		if (sA_BodyID == BodyType::DYNAMIC)
			sphereA->ApplyForce(forceVec * -(combElasticity));
	
		//apply rotational velocity
		
		glm::vec3 colRadi = collNormal * sARadius;
		glm::vec3 torqueLever = glm::normalize(glm::cross(glm::cross(relVelocity, colRadi), colRadi)) *  sARadius;
		float torque = glm::dot(torqueLever, relVelocity) * combineMass;
		glm::vec3 dir = glm::normalize(glm::cross(colRadi, relVelocity));
	
		if (sB_BodyID == BodyType::DYNAMIC)
			sphereA->ApplyTorque(-torque, dir);
		if (sA_BodyID == BodyType::DYNAMIC)
			sphereB->ApplyTorque(torque, dir);
		
		// Move our spheres out of collision
		glm::vec3 sepVec = collNormal * intersection;
	
		if (sB_BodyID == BodyType::DYNAMIC)
			sphereB->SetPosition(sPosB + sepVec * 0.5f);// glm::vec3(glm::vec2(sepVec), 1.0f));
		else if (sA_BodyID == BodyType::DYNAMIC)
			sphereA->SetPosition(sPosA - sepVec * 0.5f);// glm::vec3(glm::vec2(sepVec), 1.0f));

		if (sB_BodyID == BodyType::DYNAMIC && sA_BodyID == BodyType::STATIC)
			sphereB->SetPosition(sPosA - sepVec);
		else if (sA_BodyID == BodyType::DYNAMIC && sA_BodyID == BodyType::STATIC)
			sphereA->SetPosition(sPosA - sepVec);

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
	return PlaneToAABB(a_B, a_A);
}

bool PhysScene::AABBToSphere(PhysActor* a_A, PhysActor* a_B)
{
	return SphereToAABB(a_B, a_A);
}

bool PhysScene::AABBToAABB(PhysActor* a_A, PhysActor* a_B)
{
	PhysAABB* AABB_A = dynamic_cast<PhysAABB*>(a_A);
	PhysAABB* AABB_B = dynamic_cast<PhysAABB*>(a_B);

	glm::vec3 A_pos = AABB_A->GetPosition();
	glm::vec3 B_pos = AABB_B->GetPosition();
	glm::vec3 A_size = AABB_A->GetSize();
	glm::vec3 B_size = AABB_B->GetSize();

	glm::vec3 A_min = A_pos - A_size;
	glm::vec3 A_max = A_pos + A_size;

	glm::vec3 B_min = B_pos - B_size;
	glm::vec3 B_max = B_pos + B_size;

	// Use Newtons third law to apply collision forces to collided bodies.
	glm::vec3 delta = B_pos - A_pos;

	// Check if AABB_A's max is greater than AABB_B's min and
	// AABB_A's min is less than AABB_B's max
	if ((A_max.x > B_min.x && A_min.x < B_max.x &&
		 A_max.y > B_min.y && A_min.y < B_max.y &&
		 A_max.z > B_min.z && A_min.z < B_max.z))
	{
		// Interception
		glm::vec3 inter = glm::min(A_max - B_min, B_max - A_min);

		float minInter = glm::min(glm::min(inter.x, inter.y), inter.z);
		glm::vec3 collisionNorm;

		float delta = -1;
		int axis = -1;

		if (minInter == inter[0])
		{
			axis = 0;
			collisionNorm = glm::vec3(1, 0, 0);
			if (A_min[axis] < B_min[axis])
				delta = -1 * minInter / 2.0f;
			else
			{
				collisionNorm *= -1.0f;
				delta = minInter / 2.0f;
			}
		}
		else if (minInter == inter[1])
		{
			axis = 1;
			collisionNorm = glm::vec3(1, 0, 0);
			if (A_min[axis] < B_min[axis])
				delta = -1 * minInter / 2.0f;
			else
			{
				collisionNorm *= -1.0f;
				delta = minInter / 2.0f;
			}
		}
		else if (minInter == inter[2])
		{
			axis = 2;
			collisionNorm = glm::vec3(1, 0, 0);
			if (A_min[axis] < B_min[axis])
				delta = -1 * minInter / 2.0f;
			else
			{
				collisionNorm *= -1.0f;
				delta = minInter / 2.0f;
			}
		}

		if (AABB_B->GetBodyID() == BodyType::DYNAMIC)
		{
			if (axis == 0)
				AABB_B->SetPosition(glm::vec3(B_pos.x - delta, B_pos.y, B_pos.z));
			else if (axis == 1)
				AABB_B->SetPosition(glm::vec3(B_pos.x, B_pos.y - delta * 2.0f, B_pos.z));
			else if (axis == 2)
				AABB_B->SetPosition(glm::vec3(B_pos.x, B_pos.y, B_pos.z - delta));
		}
		else if (AABB_A->GetBodyID() == BodyType::DYNAMIC)
		{
			if (axis == 0)
				AABB_A->SetPosition(glm::vec3(B_pos.x + delta, B_pos.y, B_pos.z));
			else if (axis == 1)
				AABB_A->SetPosition(glm::vec3(B_pos.x, B_pos.y + delta * 2.0f, B_pos.z));
			else if (axis == 2)
				AABB_A->SetPosition(glm::vec3(B_pos.x, B_pos.y, B_pos.z + delta));
		}

		glm::vec3 relVel = AABB_A->GetLinearVel() - AABB_B->GetLinearVel();
		glm::vec3 colVec = collisionNorm * glm::abs(glm::dot(relVel, collisionNorm));
		glm::vec3 forceVec = colVec * 1.0f / (1 / AABB_A->GetMass() + 1 / AABB_B->GetMass());
		float combElasticity = (AABB_A->GetElasticity() + AABB_B->GetElasticity()) / 2.0f;

		if (AABB_A->GetBodyID() == BodyType::DYNAMIC)
			AABB_A->ApplyForce(forceVec * combElasticity);
		else if (AABB_B->GetBodyID() == BodyType::DYNAMIC)
			AABB_B->ApplyForce(-forceVec * combElasticity);

		return true;
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