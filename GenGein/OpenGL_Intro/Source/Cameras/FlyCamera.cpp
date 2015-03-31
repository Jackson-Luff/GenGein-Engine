
#include <stdio.h>
#include <gl_core_4_4.h>
#include <glm\gtx\rotate_vector.hpp>
#include <GLFW\glfw3.h>

#include "FlyCamera.h"

// Default Constructor
FlyCamera::FlyCamera() :
	m_speed(glm::vec3(0)),
	m_fRotSpeed(0),
	m_bViewButtonClicked(false),
	m_dCursorX(0), m_dCursorY(0)
{}

// Constructor with flying and rotational speeds
FlyCamera::FlyCamera( c_float a_minSpeed, c_float a_maxSpeed, c_float a_rotationSpeed) : BaseCamera()
{
	SetBaseSpeed(15);
	// Initialise data
	m_bViewButtonClicked = false;
	m_speed.x = a_minSpeed;
	m_speed.y = (a_maxSpeed/a_minSpeed);
	m_speed.z = a_maxSpeed;
	m_fRotSpeed = a_rotationSpeed;
}

// Deconstructor
FlyCamera::~FlyCamera() 
{}

// Update loop for look/rotation inputs
void FlyCamera::Update( c_double a_dt )
{
	// Update the handling queries
	HandleKeyboardInput( a_dt );
	HandleMouseInput( a_dt );
}

// Input Handler for the keyboard (REMOVE)
void FlyCamera::HandleKeyboardInput( c_double a_dt )
{
	//Get the cameras forward/up/right
	glm::vec4 vRight = m_worldTransform[0];
	glm::vec4 vUp	 = m_worldTransform[1];
	glm::vec4 vFrwrd = m_worldTransform[2];

	glm::vec4 moveDir(0.0f);

	//Retain a direction via with button is pressed
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
		moveDir -= vFrwrd;
	else if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
		moveDir += vFrwrd;
	else if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
		moveDir -= vRight;
	else if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
		moveDir += vRight;
	else if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
		moveDir += glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	else if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
		moveDir -= glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	m_speed.y = GetBaseSpeed();

	// Allow for speed boost
	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		SetCurrFlySpeed( 10* GetMaxFlySpeed() );
	// Smoothly decend from speed boost and clip min speed
	else if (GetCurrFlySpeed() >= GetMaxFlySpeed())
		SetCurrFlySpeed(GetCurrFlySpeed() * 0.98f);

	if(GetCurrFlySpeed() <= GetMinFlySpeed())
		SetCurrFlySpeed(GetMinFlySpeed());

	//Apply movement to the current position
	if (glm::length(moveDir) > 0.01f)
	{		
		moveDir = ((float)a_dt * GetCurrFlySpeed()) * glm::normalize(moveDir);
		SetPosition( GetPosition() + moveDir);
	}
}

// Input Handler for the mouse (REMOVE)
void FlyCamera::HandleMouseInput( c_double a_dt )
{
	// Check for Right mouse key clicked
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		// Check for held down
		if (m_bViewButtonClicked == false)
		{
			// Set center cursor to of screen
			int width, height;
			glfwGetFramebufferSize(m_pWindow, &width, &height);

			m_dCursorX = width / 2.0;
			m_dCursorY = height / 2.0;
			glfwSetCursorPos(m_pWindow, m_dCursorX, m_dCursorY);
			m_bViewButtonClicked = true;
		}
		else
		{
			// Retrieve current mouse position
			double mouseX, mouseY;
			glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);

			//Calculate delta between new and old mouse position
			double dx = mouseX - m_dCursorX;
			double dy = mouseY - m_dCursorY;

			// Store old mouse position
			m_dCursorX = mouseX;
			m_dCursorY = mouseY;

			// Calculate the rotation
			CalculateRotation( a_dt, dx, dy);
		}
	}
	else
		m_bViewButtonClicked = false;
}

// Calculates the rotation based on mouse delta
void FlyCamera::CalculateRotation( c_double a_dt, c_double a_dx, c_double a_dy )
{
	// Calculate the rotation of the delta vector
	glm::mat3 xRot = glm::mat3(glm::rotate( (float)(a_dx * (a_dt * -GetRotSpeed())),glm::vec3(0,1,0)));
	glm::mat3 yRot = glm::mat3(glm::rotate( (float)(a_dy * (a_dt * -GetRotSpeed())),glm::vec3(1,0,0)));

	// Make sure the magnitude is larger than 0 (divide by 0 error)
	if(glm::length(glm::vec2(a_dx, a_dy)) > 0.01f)
		SetWorldTransform( GetWorldTransform() * glm::mat4(xRot * yRot) );

	// Create a side vector and then from that an up vector 
	glm::vec3 m_sideVector = glm::cross( glm::vec3(0,1,0), glm::vec3(m_worldTransform[2]) );
	glm::vec3 m_upVector   = glm::cross(     m_sideVector, glm::vec3(m_worldTransform[2]) );

	// Normalise the two
	m_sideVector = glm::normalize(m_sideVector);
	m_upVector   = glm::normalize(  m_upVector);

	// Apply to the world Transform 
	m_worldTransform[0] = glm::vec4(m_sideVector, 0);
	m_worldTransform[1] = glm::vec4(  -m_upVector, 0);
	m_worldTransform[2] = glm::normalize(m_worldTransform[2]);
	// Apply View Transform 
	m_viewTransform = glm::inverse( GetWorldTransform() );
}