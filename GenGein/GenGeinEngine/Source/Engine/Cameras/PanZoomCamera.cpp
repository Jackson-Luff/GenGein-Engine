
#include <stdio.h>
#include <gl_core_4_4.h>
#include <glm\gtx\rotate_vector.hpp>
#include <GLFW\glfw3.h>

#include "PanZoomCamera.h"

// Default Constructor
PanZoomCamera::PanZoomCamera() :
m_speed(0),
m_fRotSpeed(0),
m_bViewButtonClicked(false),
m_dCursorX(0), m_dCursorY(0),
tempTimer(0)
{}

// Constructor with flying and rotational speeds
PanZoomCamera::PanZoomCamera(const float32_t& a_moveSpeed, const float32_t& a_rotSpeed) 
	: BaseCamera()
{
	SetBaseSpeed(a_moveSpeed);
	// Initialise data
	m_bViewButtonClicked = false;
	m_speed = a_moveSpeed;
	m_fRotSpeed = a_rotSpeed;
	tempTimer = 0;
}

// Deconstructor
PanZoomCamera::~PanZoomCamera()
{}

// Update loop for look/rotation inputs
void PanZoomCamera::Update(const double_t& a_dt)
{
	// Update the handling queries
	HandleKeyboardInput(a_dt);
	HandleMouseInput(a_dt);
}

// Input Handler for the keyboard (REMOVE)
void PanZoomCamera::HandleKeyboardInput(const double_t& a_dt)
{
	//Get the cameras forward/up/right
	float32_t dist = glm::length(m_worldTrans[2] - GetPosition());

	if (dist < 0.01)
		return;
	
	f32vec4 moveDir(0.0f);
	float32_t div = 0.0f;
	//tempTimer += (float32_t)a_dt;

	//Retain a direction via with button is pressed
	/*if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		LookAt(vec3(0));

		moveDir.x = -sin(tempTimer);
		moveDir.z = -cos(tempTimer);
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		LookAt(vec3(0));
		
		moveDir.x = sin(tempTimer);
		moveDir.z = cos(tempTimer);
	}*/

	//Circulate just above the given LookAt
	//=> x = x + 6 * sin(time)
	//=> z = z + 6 * cos(time)

	m_speed = GetBaseSpeed();

	//Apply movement to the current position
	if (glm::length(moveDir) > 0.01f)
	{
		moveDir = ((float32_t)a_dt * m_speed) * glm::normalize(moveDir);
		SetPosition(GetPosition() + moveDir);
	}
}

// Input Handler for the mouse (REMOVE)
void PanZoomCamera::HandleMouseInput(const double_t& a_dt)
{
	// Check for Right mouse key clicked
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		// Check for held down
		if (m_bViewButtonClicked == false)
		{
			// Set center cursor to of screen
			int32_t width, height;
			glfwGetFramebufferSize(m_pWindow, &width, &height);

			m_dCursorX = width / 2.0;
			m_dCursorY = height / 2.0;
			glfwSetCursorPos(m_pWindow, m_dCursorX, m_dCursorY);
			m_bViewButtonClicked = true;
		}
		else
		{
			// Retrieve current mouse position
			double_t mouseX, mouseY;
			glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);

			//Calculate delta between new and old mouse position
			double_t dx = mouseX - m_dCursorX;
			double_t dy = mouseY - m_dCursorY;

			// Store old mouse position
			m_dCursorX = mouseX;
			m_dCursorY = mouseY;

			// Calculate the rotation
			CalculateRotation(a_dt, dx, dy);
		}
	}
	else
		m_bViewButtonClicked = false;
}

// Calculates the rotation based on mouse delta
void PanZoomCamera::CalculateRotation(const double_t& a_dt, const double_t& a_dx, const double_t& a_dy)
{
	// Calculate the rotation of the delta vector
	f32mat3 xRot = f32mat3(rotate((float32_t)(a_dx * (a_dt * -GetRotSpeed())), f32vec3(0, 1, 0)));
	f32mat3 yRot = f32mat3(rotate((float32_t)(a_dy * (a_dt * -GetRotSpeed())), f32vec3(1, 0, 0)));

	// Make sure the magnitude is larger than 0 (divide by 0 error)
	if (length(f32vec2(a_dx, a_dy)) > 0.01f)
		SetWorldTransform(GetWorldTransform() * f32mat4(xRot * yRot));

	// Create a side vector and then from that an up vector 
	f32vec3 m_sideVector = cross(f32vec3(0, 1, 0), f32vec3(m_worldTrans[2]));
	f32vec3 m_upVector = cross(m_sideVector, f32vec3(m_worldTrans[2]));

	// Normalise the two
	m_sideVector = normalize(m_sideVector);
	m_upVector = normalize(m_upVector);

	// Apply to the world Transform 
	m_worldTrans[0] = f32vec4(m_sideVector, 0);
	m_worldTrans[1] = f32vec4(-m_upVector, 0);
	m_worldTrans[2] = normalize(m_worldTrans[2]);
	// Apply View Transform 
	m_viewTrans = inverse(GetWorldTransform());
}