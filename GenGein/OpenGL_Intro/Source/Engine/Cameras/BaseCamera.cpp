
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "BaseCamera.h"

// Default Constructor
BaseCamera::BaseCamera()
: m_viewTrans(f32mat4(1)),
  m_projectionTrans(f32mat4(1)),
  m_worldTrans(f32mat4(1)),
  m_bPerspectiveSet(false)
{
	// Update the projection view 
	UpdateProjectionViewTransform();
}

// Constructs a camera based on a position
BaseCamera::BaseCamera(const f32vec3& a_pos)
{
	// Set curr speed;
	m_currentSpeed = 0;
	// Apply position within world
	m_worldTrans[3] = f32vec4( a_pos, 1 );
	// Perspective Hasn't been set yet, thus false.
	m_bPerspectiveSet = false;
	// Update the projection view 
	UpdateProjectionViewTransform();
}

// Constructs a camera based on a matrix
BaseCamera::BaseCamera(const f32mat4& a_trans)
{
	// Set curr speed;
	m_currentSpeed = 0;
	// Set the world transform
	SetWorldTransform( a_trans );
	// Update the projection view 
	UpdateProjectionViewTransform();
}

// Deconstructor
BaseCamera::~BaseCamera() 
{}

// Override world transform
void BaseCamera::SetWorldTransform(const f32mat4& a_trans)
{
	// Set the world transform
	m_worldTrans = a_trans;
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Override position within world transform
void BaseCamera::SetPosition(const f32vec4& a_pos)
{
	// Set Position within world
	m_worldTrans[3] = a_pos;
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Look at a point with defaulted position
void BaseCamera::LookAt(const f32vec3& a_lkAt)
{
	// Apply LookAt without a position
	m_worldTrans = inverse(lookAt(f32vec3(m_worldTrans[3]), a_lkAt, f32vec3(0,1,0)));
	UpdateProjectionViewTransform();
}

// Look at a point based on a position
void BaseCamera::LookAt(const f32vec3& a_pos, const f32vec3& a_lkAt)
{
	// Apply LookAt with a position
	m_worldTrans = inverse(lookAt(a_pos, a_lkAt, f32vec3(0,1,0)));
	UpdateProjectionViewTransform();
}

// Initialise the perspective
void BaseCamera::InitialisePerspective(
	const float32_t& a_fov,
	const float32_t& a_aspR,
	const float32_t& a_nrPln,
	const float32_t& a_frPln)
{
	// Initialise the projection transform 
	m_projectionTrans = perspective(a_fov, a_aspR, a_nrPln, a_frPln);
	m_bPerspectiveSet = true;
	
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Returns a world-space normalized vector pointing away from the camera's world-space position
f32vec3 BaseCamera::ScreenPositionToDirection(const float32_t& x, const float32_t& y)
{
	int32_t width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	f32vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projectionTrans[0][0];
	screenPos.y /= m_projectionTrans[1][1];

	return normalize(m_worldTrans * f32vec4(screenPos, 0)).xyz();
}

// Returns the point of interscetion of a camera ray and a world-space plane
// the plane has a normal of XYZ and is offset from (0,0,0) by -W in the direction of the normal
f32vec3 BaseCamera::PickAgainstPlane(const f32vec4& plane) const
{
	// Extract cursor position
	double_t x = 0, y = 0;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	
	// Extract window measurements
	int32_t width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	// Convert cursor position into OpenGL coordinates (-1 to 1)
	f32vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);
	
	//
	screenPos.x /= m_projectionTrans[0][0];
	screenPos.y /= m_projectionTrans[1][1];

	//
	f32vec3 dir = normalize(m_worldTrans * vec4(screenPos, 0)).xyz();
	
	// Line-Plane Intersection method:
	// Reference: http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	// (P - Po) • N = 0
	// ((dL + Lo) - Po) • N = 0
	// ((dL + 'CamPos') - ('0,0.73,0')) • N = 0
	// (dL)• N + ('CamPos' - Po) • N = 0
	// d = (Po - Lo) • N / L • N
	// d = ((plane.xyz() * plane.w) - 'CamPos') • plane.xyz() / dir • plane.xyz()
	float32_t PoSubLoDotN = dot((plane.xyz() * plane.w) - m_worldTrans[3].xyz(), plane.xyz());
	float32_t LDotN = dot(dir, plane.xyz());
	// thus:
	float32_t d = PoSubLoDotN / LDotN;

	// 
	return m_worldTrans[3].xyz() + dir * d;
}

// Constant updating for the projection view matrix
void BaseCamera::UpdateProjectionViewTransform()
{
	// Inverse the world transform to get the view transform
	m_viewTrans = inverse(m_worldTrans);
	// Apply the projection and view transform into the projectionView transform
	m_projectionViewTrans = m_projectionTrans * m_viewTrans;
}
