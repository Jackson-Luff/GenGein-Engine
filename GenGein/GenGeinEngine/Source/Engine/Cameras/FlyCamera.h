#pragma once

#include "BaseCamera.h"

////
// Author: Jackson Luff
// Name: Fly Camera
// Type: class (standard)
// Parent: Base Camera
// Description:
// * The fly camera is your typical 'spectator' 
// * camera. You can fly around freely throughout
// * the scene.
////
class FlyCamera : public BaseCamera
{
public:
	// Constructors
	FlyCamera();
	FlyCamera(const float32_t& a_minSpeed,
		const float32_t& a_maxSpeed,
		const float32_t& a_rotationSpeed);
	// Deconstructor
	~FlyCamera();
	// Update loop
	virtual void Update( const double_t& a_dt );
	
	// Get / Set the minimum fly speed
	inline const float32_t& GetMinFlySpeed() const 
		{ return m_speed.x; }
	inline void SetMinFlySpeed(const float32_t& a_fSpeed) 
		{ m_speed.x = a_fSpeed; }

	// Get / Set the current fly speed
	inline const float32_t& GetCurrFlySpeed() const 
		{ return m_speed.y; }
	inline void SetCurrFlySpeed(const float32_t& a_fSpeed)
		{ m_speed.y = a_fSpeed; }

	// Get / Set the maximum fly speed
	inline const float32_t& GetMaxFlySpeed() const 
		{ return m_speed.z; }
	inline void SetMaxFlySpeed(const float32_t& a_fSpeed)	
		{ m_speed.z = a_fSpeed; }

	// Get / Set the camera rotation
	inline const float32_t& GetRotSpeed() const 
		{ return m_fRotSpeed; }
	inline void SetRotSpeed(const float32_t& fSpeed) 
		{ m_fRotSpeed = fSpeed; }

protected:
	// Input Handles
	void HandleKeyboardInput( const double_t& a_dt );
	void HandleMouseInput(const double_t& a_dt);
	// Calculate the rotation of the camera
	void CalculateRotation(const double_t& a_dt, const double_t& a_dx, const double_t& a_dy);
	// Speed variant (x : min, y : curr, z : max)
	f32vec3 m_speed;
	// Rotational speed
	float32_t m_fRotSpeed;
	// Mouse is down boolean
	bool m_bViewButtonClicked;
	// Previous Mouse coordinates
	double_t m_dCursorX, m_dCursorY;
};