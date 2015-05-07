#pragma once 
#include <glm\glm.hpp>

struct GLFWwindow;

typedef const glm::vec3 c_vec3;
typedef const glm::vec4 c_vec4;
typedef const glm::mat4 c_mat4;
typedef const unsigned int c_uint;
typedef const float c_float;

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
	inline c_mat4& GetWorldTransform() const { return m_worldTrans; }
	void SetWorldTransform( c_mat4 transform );
	// Get / Set Position
	inline c_vec4& GetPosition() const { return m_worldTrans[3]; }
	void SetPosition( c_vec4 position );
	// Gets / sets current speed
	inline c_float& GetBaseSpeed() const { return m_currentSpeed; }
	inline float SetBaseSpeed(c_float a_newSpeed) { return m_currentSpeed = a_newSpeed; }

	// Multiple LookAt functions for varied arguments
	void LookAt( c_vec3 lookAt );
	void LookAt( c_vec3 position, c_vec3 lookAt);
	// Initialise the Perspective
	void InitialisePerspective( c_float fov, c_float aspR, c_float near = 0.1f, c_float far = 1000.0f);

	// Returns if the perspective has been initialised
	inline const bool GetPerspectiveSet() const { return m_bPerspectiveSet;  }
	// Set Input Window (REMOVE)
	inline void SetInputWindow( GLFWwindow* a_pWindow ) { m_pWindow = a_pWindow; }
	
	// Returns a world-space normalized vector pointing away from the camera's world-space position
	glm::vec3 ScreenPositionToDirection(c_float x, c_float y);
	// Returns the point of interscetion of a camera ray and a world-space plane
	// the plane has a normal of XYZ and is offset from (0,0,0) by -W in the direction of the normal
	glm::vec3 PickAgainstPlane(c_float x, c_float y, c_vec4& plane) const;

	// Getters to retrieve the projection, view and projectionView transforms
	inline c_mat4& GetProjection()	 const { return m_projectionTrans; }
	inline c_mat4& GetView()		 const { return m_viewTrans; }
	inline c_mat4& GetProjectionView() const { return m_projectionViewTrans; }

	// Setters to retrieve the projection, view and projectionView transforms
	inline void SetProjection(c_mat4 a_newProj) { m_projectionTrans = a_newProj; }
	inline void SetView(c_mat4& a_newView)		{ m_viewTrans = a_newView; }
	inline void SetProjectionView(c_mat4& a_newProjView) { m_projectionViewTrans = a_newProjView; }

protected:
	// Update projection view (per frame)
	void UpdateProjectionViewTransform();

	// Camera's internal transforms
	glm::mat4 m_viewTrans;
	glm::mat4 m_projectionTrans;
	glm::mat4 m_projectionViewTrans;

	// Camera's world transform
	glm::mat4 m_worldTrans;

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