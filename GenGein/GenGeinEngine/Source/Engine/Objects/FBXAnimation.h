#pragma once
#include <string>
#include <glm\glm.hpp>

#include <FBXFile.h>

typedef unsigned int uint;

// An animation that contains a collection of animated bone tracks
class FBXAnimation
{
public:
	FBXAnimation();
	~FBXAnimation();
	//creates a deep-copy of this animation
	FBXAnimation* clone() const;
	unsigned int totalFrames() const;
	float totalTime(float a_fps = 24.0f) const;
	std::string m_name;
	uint m_startFrame;
	uint m_endFrame;
	uint m_trackCount;
	FBXTrack* m_tracks;
};

// A hierarchy of bones that can be animated
class FBXSkeleton
{
public:
	FBXSkeleton();
	~FBXSkeleton();
	void evaluate(const FBXAnimation* a_animation,
		float a_time,
		bool a_loop = true,
		float a_fps = 24.0f);
	void updateBones();
	uint m_boneCount;
	FBXNode** m_nodes;
	int* m_parentIndex;
	//ready for use in skinning! (bind pose combined)
	glm::mat4* m_bones;
	glm::mat4* m_bindPoses;
	void* m_userData;
};

// A single frame for a bone in an animation
class FBXKeyFrame
{
public:
	FBXKeyFrame();
	~FBXKeyFrame();
	uint m_key;
	glm::quat m_rotation;
	glm::vec3 m_translation;
	glm::vec3 m_scale;
};

// A collection of frames for a single bone in an animation
class FBXTrack
{
public:
	FBXTrack();
	~FBXTrack();
	uint m_boneIndex;
	uint m_keyframeCount;
	FBXKeyFrame* m_keyframes;
};