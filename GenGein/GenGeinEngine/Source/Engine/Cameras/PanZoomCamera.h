#pragma once

#include "BaseCamera.h"

////
// Author: Jackson Luff
// Name: Pan-Zoom Camera
// Type: class (standard)
// Parent: Base Camera
// Description:
// * This camera is simply a pan-zoom
// * of it's type. It can pan and zoom
// * for a specific lookat location.
////
class PanZoomCamera : public BaseCamera
{
public:
	// Constructors
	PanZoomCamera();
	PanZoomCamera(const float32_t& a_moveSpeed, const float32_t& a_rotSpeed);
	// Deconstructor
	~PanZoomCamera();
	// Update loop
	virtual void Update(const double_t& a_dt);

	// Set fly speed of camera pan
	inline const float32_t& GetMoveSpeed() const
		{ return m_speed; }

	// Get / Set the camera rotation
	inline const float32_t& GetRotSpeed() const
		{ return m_fRotSpeed; }
	inline void SetRotSpeed(const float32_t& a_speed)
		{ m_fRotSpeed = a_speed; }

protected:
	// Input Handles
	void HandleKeyboardInput(const double_t& a_dt);
	void HandleMouseInput(const double_t& a_dt);
	// Calculate the rotation of the camera
	void CalculateRotation(const double_t& a_dt, const double_t& a_dx, const double_t& a_dy);
	// Movement speed
	float32_t m_speed;
	// Rotational speed
	float32_t m_fRotSpeed;
	// Mouse is down boolean
	bool m_bViewButtonClicked;
	// Previous Mouse coordinates
	double_t m_dCursorX, m_dCursorY;

	float32_t tempTimer;
};