
#include <gl_core_4_4.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "BaseCamera.h"

// Default Constructor
BaseCamera::BaseCamera()
: m_viewTransform(glm::mat4(1)),
  m_projectionTransform(glm::mat4(1)),
  m_worldTransform(glm::mat4(1)),
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
	m_worldTransform[3] = glm::vec4( a_pos, 1 );
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
	m_worldTransform = a_trans;
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Override position within world transform
void BaseCamera::SetPosition( c_vec4 a_pos )
{
	// Set Position within world
	m_worldTransform[3] = a_pos;
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Look at a point with defaulted position
void BaseCamera::LookAt( c_vec3 a_lkAt)
{
	// Apply LookAt without a position
	m_worldTransform = glm::inverse(glm::lookAt(glm::vec3(m_worldTransform[3]), a_lkAt, glm::vec3(0,1,0)));
	UpdateProjectionViewTransform();
}

// Look at a point based on a position
void BaseCamera::LookAt( c_vec3 a_pos, c_vec3 a_lkAt)
{
	// Apply LookAt with a position
	m_worldTransform = glm::inverse(glm::lookAt(a_pos, a_lkAt, glm::vec3(0,1,0)));
	UpdateProjectionViewTransform();
}

// Initialise the perspective
void BaseCamera::InitialisePerspective( const float a_fov, const float a_aspR, const float a_nrnt, const float a_fr )
{
	// Initialise the projection transform 
	m_projectionTransform = glm::perspective(a_fov, a_aspR, a_nrnt, a_fr);
	m_bPerspectiveSet = true;
	
	// Update the projection view
	UpdateProjectionViewTransform();
}

// Constant updating for the projection view matrix
void BaseCamera::UpdateProjectionViewTransform()
{
	// Inverse the world transform to get the view transform
	m_viewTransform = glm::inverse(m_worldTransform);
	// Apply the projection and view transform into the projectionView transform
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
