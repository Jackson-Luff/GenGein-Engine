#pragma once

#include "BaseCamera.h"

typedef const float c_float;
typedef const double c_double;
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
	FlyCamera(c_float a_minSpeed, c_float a_maxSpeed, c_float a_rotationSpeed);
	// Deconstructor
	~FlyCamera();
	// Update loop
	virtual void Update( c_double a_dt );
	
	// Get / Set the minimum fly speed
	inline c_float& GetMinFlySpeed() const { return m_speed.x; }
	inline void SetMinFlySpeed( c_float a_fSpeed ) { m_speed.x = a_fSpeed; }
	// Get / Set the current fly speed
	inline c_float& GetCurrFlySpeed() const { return m_speed.y; }
	inline void SetCurrFlySpeed( c_float a_fSpeed ) { m_speed.y = a_fSpeed; }
	// Get / Set the maximum fly speed
	inline c_float& GetMaxFlySpeed() const { return m_speed.z; }
	inline void SetMaxFlySpeed( c_float a_fSpeed ) { m_speed.z = a_fSpeed; }
	// Get / Set the camera rotation
	inline c_float& GetRotSpeed() const { return m_fRotSpeed; }
	inline void SetRotSpeed( c_float fSpeed ) { m_fRotSpeed = fSpeed; }

protected:
	// Input Handles
	void HandleKeyboardInput( c_double a_dt );
	void HandleMouseInput( c_double a_dt );
	// Calculate the rotation of the camera
	void CalculateRotation( c_double a_dt, c_double a_dx, c_double a_dy );
	// Speed variant (x : min, y : curr, z : max)
	glm::vec3 m_speed;
	// Rotational speed
	float m_fRotSpeed;
	// Mouse is down boolean
	bool m_bViewButtonClicked;
	// Previous Mouse coordinates
	double m_dCursorX, m_dCursorY;
};