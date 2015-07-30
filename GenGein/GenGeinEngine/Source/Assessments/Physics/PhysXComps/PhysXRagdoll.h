#pragma once
#include <PxPhysics.h>

namespace RagDoll
{
	// Parts which make up our ragdoll
	enum RagDollParts
	{
		NO_PARENT = -1,
		LOWER_SPINE,
		LEFT_PELVIS,
		RIGHT_PELVIS,
		LEFT_UPPER_LEG,
		RIGHT_UPPER_LEG,
		LEFT_LOWER_LEG,
		RIGHT_LOWER_LEG,
		UPPER_SPINE,
		LEFT_CLAVICLE,
		RIGHT_CLAVICLE,
		NECK,
		HEAD,
		LEFT_UPPER_ARM,
		RIGHT_UPPER_ARM,
		LEFT_LOWER_ARM,
		RIGHT_LOWER_ARM
	};

	struct RagDollNode
	{
		// Rotation of this link in model space
		physx::PxQuat globalRot;

		// Position of the link centre in world space
		// which is calculated when we process the node
		physx::PxVec3 scaledGlobalPos;
		//Index of the parent node
		int parentNodeIdx;
		// Half length of the capsule for this node
		float halfLength;
		// Radius of the capsule for this node
		float radius;
		// relative position of link center in parent to this node
		// |  0 is the center of the node's x
		// | -1 is left of the capsule relative to x
		// | +1 is right of the capsule relative to x
		float parentLinkPos;
		// Relative position of link center in child
		float childLinkPos;
		// Name of link
		char* name;

		physx::PxArticulationLink* linkPtr;

		RagDollNode(physx::PxQuat a_glblRot, int a_prntNodeIdx, float a_halfLength,
			float a_radius, float a_prntLinkPos, float a_chldLinkPos, char* a_name) :
			globalRot(a_glblRot), parentNodeIdx(a_prntNodeIdx),
			halfLength(a_halfLength), radius(a_radius), parentLinkPos(a_prntLinkPos),
			childLinkPos(a_chldLinkPos), name(a_name)
		{}
	};
}
