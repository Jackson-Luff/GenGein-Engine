
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "BaseCamera.h"

// Default Constructor
BaseCamera::BaseCamera()
: m_viewTrans(glm::mat4(1)),
  m_projectionTrans(glm::mat4(1)),
  m_worldTrans(glm::mat4(1)),
  m_bPerspectiveSet(false)
{
	// Update the projection view 
	UpdateProjectionViewTransform();
}

// Constructs a camera based on a position
BaseCamera::BaseCamera(c_vec3 a_pos)
{
	// Set curr speed;
	m_currentSpeed = 0;
	// Apply position within world
	m_worldTrans[3] = glm::vec4( a_pos, 1 );
	// Perspective Hasn't been set yet, thus false.
	m_bPerspectiveSet = false;
	// Update the projection view 
	UpdateProjectionViewTransform();
}

// Constructs a camera based on a matrix
BaseCamera::BaseCamera(c_mat4 a_trans)
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
void BaseCamera::SetWorldTransform( c_mat4 a_trans )
{
	// Set the world transform
	m_worldTrans = a_trans;
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Override position within world transform
void BaseCamera::SetPosition( c_vec4 a_pos )
{
	// Set Position within world
	m_worldTrans[3] = a_pos;
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Look at a point with defaulted position
void BaseCamera::LookAt( c_vec3 a_lkAt)
{
	// Apply LookAt without a position
	m_worldTrans = glm::inverse(glm::lookAt(glm::vec3(m_worldTrans[3]), a_lkAt, glm::vec3(0,1,0)));
	UpdateProjectionViewTransform();
}

// Look at a point based on a position
void BaseCamera::LookAt( c_vec3 a_pos, c_vec3 a_lkAt)
{
	// Apply LookAt with a position
	m_worldTrans = glm::inverse(glm::lookAt(a_pos, a_lkAt, glm::vec3(0,1,0)));
	UpdateProjectionViewTransform();
}

// Initialise the perspective
void BaseCamera::InitialisePerspective( const float a_fov, const float a_aspR, const float a_nrnt, const float a_fr )
{
	// Initialise the projection transform 
	m_projectionTrans = glm::perspective(a_fov, a_aspR, a_nrnt, a_fr);
	m_bPerspectiveSet = true;
	
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Returns a world-space normalized vector pointing away from the camera's world-space position
glm::vec3 BaseCamera::ScreenPositionToDirection(c_float x, c_float y)
{
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projectionTrans[0][0];
	screenPos.y /= m_projectionTrans[1][1];

	return glm::normalize(m_worldTrans * glm::vec4(screenPos, 0)).xyz();
}

// Returns the point of interscetion of a camera ray and a world-space plane
// the plane has a normal of XYZ and is offset from (0,0,0) by -W in the direction of the normal
glm::vec3 BaseCamera::PickAgainstPlane(c_vec4& plane) const
{
	double x = 0, y = 0;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);
	
	screenPos.x /= m_projectionTrans[0][0];
	screenPos.y /= m_projectionTrans[1][1];

	glm::vec3 dir = glm::normalize(m_worldTrans * glm::vec4(screenPos, 0)).xyz();
	printf("%f, %f, %f\n", dir.x, dir.y, dir.z);

	float d = (plane.w - glm::dot(m_worldTrans[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	//glm::vec3 pos = m_worldTrans[3].xyz() + dir * d;
	//printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
	return m_worldTrans[3].xyz() + dir * d;
}

// Constant updating for the projection view matrix
void BaseCamera::UpdateProjectionViewTransform()
{
	// Inverse the world transform to get the view transform
	m_viewTrans = glm::inverse(m_worldTrans);
	// Apply the projection and view transform into the projectionView transform
	m_projectionViewTrans = m_projectionTrans * m_viewTrans;
}
