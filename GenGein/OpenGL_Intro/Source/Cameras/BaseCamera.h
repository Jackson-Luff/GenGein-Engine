#pragma once 
#include <glm\glm.hpp>

struct GLFWwindow;

typedef const glm::vec3 c_vec3;
typedef const glm::vec4 c_vec4;
typedef const glm::mat4 c_mat4;
typedef const unsigned int c_uint;
////
// Author: Jackson Luff
// Name: Base Camera
// Type: class (standard)
// Parent: None
// Description:
// * The BaseCamera will be the 'data container' that
// * varied camera types will inherit. This class allows
// * the user to access non-specific camera information.
////
class BaseCamera
{
public:
	// Constructors
	BaseCamera();
	BaseCamera( c_vec3 a_position );
	BaseCamera( c_mat4 transform  );
	// Deconstructor
	~BaseCamera();

	// Inheritable update
	virtual void Update( const double dt) = 0;

	// Get / Set World's Matrix Transform
	inline c_mat4& GetWorldTransform() const { return m_worldTransform; }
	void SetWorldTransform( c_mat4 transform );
	// Get / Set Position
	inline c_vec4& GetPosition() const { return m_worldTransform[3]; }
	void SetPosition( c_vec4 position );
	// Gets / sets current speed
	inline const float& GetBaseSpeed() const { return m_currentSpeed; }
	inline float SetBaseSpeed(const float a_newSpeed) { return m_currentSpeed = a_newSpeed; }

	// Multiple LookAt functions for varied arguments
	void LookAt( c_vec3 lookAt );
	void LookAt( c_vec3 position, c_vec3 lookAt);
	// Initialise the Perspective
	void InitialisePerspective( const float fov, const float aspR, const float near = 0.1f, const float far = 1000.0f);

	// Returns if the perspective has been initialised
	inline const bool GetPerspectiveSet() const { return m_bPerspectiveSet;  }
	// Set Input Window (REMOVE)
	inline void SetInputWindow( GLFWwindow* a_pWindow ) { m_pWindow = a_pWindow; }
	
	// Getters to retrieve the projection, view and projectionView transforms
	inline c_mat4& GetProjection()	 const { return m_projectionTransform; }
	inline c_mat4& GetView()		 const { return m_viewTransform; }
	inline c_mat4& GetProjectionView() const { return m_projectionViewTransform; }
protected:
	// Update projection view (per frame)
	void UpdateProjectionViewTransform();

	// Camera's internal transforms
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;

	// Camera's world transform
	glm::mat4 m_worldTransform;

	// Trigger to test if perspective is set
	bool m_bPerspectiveSet;	
	// Window reference (REMOVE)
	GLFWwindow* m_pWindow;
	// EyePos uniform
	int eyePosUniform;
private:
	
	// Stores current speed from all camera's
	float m_currentSpeed;
};