#pragma once 
#include <glm\glm.hpp>

struct GLFWwindow;

using namespace glm;

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
	BaseCamera( const f32vec3& a_position );
	BaseCamera( const f32mat4& a_transform );
	// Deconstructor
	~BaseCamera();

	// Inheritable update
	virtual void Update(const double_t& a_dt) {};

	// Get / Set World's Matrix Transform
	inline const f32mat4& GetWorldTransform() const 
		{ return m_worldTrans; }
	void SetWorldTransform(const f32mat4& a_transform);
	// Get / Set Position
	inline const f32vec4& GetPosition() const 
		{ return m_worldTrans[3]; }
	void SetPosition( const f32vec4& a_position );
	// Gets / sets current speed
	inline const float32_t& GetBaseSpeed() const { return m_currentSpeed; }
	inline float SetBaseSpeed(const float32_t& a_newSpeed) { return m_currentSpeed = a_newSpeed; }

	// Multiple LookAt functions for varied arguments
	void LookAt(const f32vec3& a_lookAt );
	void LookAt(const f32vec3& a_position, const f32vec3& a_lookAt);

	// Initialise the Perspective
	void InitialisePerspective(
		const float32_t& a_fov,
		const float32_t& a_aspR, 
		const float32_t& a_nearPlane = 0.1f,
		const float32_t& a_farPlane = 1000.0f);

	// Returns if the perspective has been initialised
	inline const bool GetPerspectiveSet() const { return m_bPerspectiveSet;  }
	// Set Input Window (REMOVE)
	inline void SetInputWindow( GLFWwindow* a_pWindow ) { m_pWindow = a_pWindow; }
	
	// Returns a world-space normalized vector pointing away from the camera's world-space position
	vec3 ScreenPositionToDirection(const float32_t& x, const float32_t& y);
	// Returns the point of interscetion of a camera ray and a world-space plane
	// the plane has a normal of XYZ and is offset from (0,0,0) by -W in the direction of the normal
	vec3 PickAgainstPlane(const f32vec4& plane) const;

	// Getters to retrieve the projection, view and projectionView transforms
	inline const f32mat4& GetProjection()		const { return m_projectionTrans; }
	inline const f32mat4& GetView()				const { return m_viewTrans; }
	inline const f32mat4& GetProjectionView()	const { return m_projectionViewTrans; }

	// Setters to retrieve the projection, view and projectionView transforms
	inline const void SetProjection(const f32mat4& a_newProj) 
		{ m_projectionTrans = a_newProj; }
	inline const void SetView(const f32mat4& a_newView)
		{ m_viewTrans = a_newView; }
	inline const void SetProjectionView(const f32mat4& a_newProjView)
		{ m_projectionViewTrans = a_newProjView; }

protected:
	// Update projection view (per frame)
	void UpdateProjectionViewTransform();

	// Camera's internal transforms
	f32mat4 m_viewTrans;
	f32mat4 m_projectionTrans;
	f32mat4 m_projectionViewTrans;

	// Camera's world transform
	f32mat4 m_worldTrans;

	// Trigger to test if perspective is set
	bool m_bPerspectiveSet;	
	// Window reference (REMOVE)
	GLFWwindow* m_pWindow;
private:
	// Stores current speed from all camera's
	float32_t m_currentSpeed;
};