#pragma once
#include <string>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

struct GLFWwindow;
class BaseCamera;
class AntTweak;
class SkyBox;

typedef const int c_int;
typedef unsigned int uint;
typedef const char* c_charp;
typedef const float c_float;
////
// Author: Jackson Luff
// Name: GLApplication
// Type: class (standard)
// Parent: None
// Description:
// * GLApplication contains all the nasty openGL
// * away from the user to allow a smooth flow 
// * throughout this framework.
////
class GLApplication
{
public:
	// Constructor
	GLApplication();
	GLApplication(c_int a_width, c_int a_height, c_charp a_title);
	// Deconstructor
	~GLApplication();

	// Initialise GLFW window
	bool InitialiseGL();
	
	// Getter for FPS count
	inline const double& GetFPS() const
		{ return m_FPS; }
	// Getter for elapsed gametime 
	inline const double GetElapsedTime() const 
		{ return m_elapsedTime; }
	// Getter for Delta Time
	inline const double GetDeltaTime() const 
		{ return m_deltaTime; }
	// Getter for MousePosition
	inline const glm::dvec2 GetMousePosition() const
	{
		double x = 0, y = 0;
		glfwGetCursorPos(m_pWindow, &x, &y);
		return glm::dvec2(x, y);
	}

	// Initialised content 
	virtual void StartUp() = 0;
	// Deconstructed content
	virtual void ShutDown() = 0;
	// Update loop
	virtual void Update( const double a_dt ) = 0;
	// Render to GUI
	virtual void Render() = 0;
	// Run the application
	void Run();

protected:
	// REMOVE:Temp hotkeys for debugging
	void DebugControls();
	// Calculate deltaTime and elapsedTiming
	void CalculateTiming();
	
	// Return the width and/or height of the window
	c_int GetWidth() const { return m_width; }
	c_int GetHeight() const { return m_height; }
	
	// Return the size of the window
	inline const glm::ivec2 GetWindowSize() const { return glm::ivec2(m_width, m_height); }
	inline void SetWindowSize( c_int a_newWidth, c_int a_newHeight)
	{ glfwSetWindowSize(m_pWindow, a_newWidth, a_newHeight); }
	
	// Sends camera data to ALL shaderprograms
	void ApplyCameraUniformSetup();

	// Set-up basic camera data
	void ApplyLightingSetup(
		const glm::vec4& a_ambient,
		const glm::vec4& a_diffuse = glm::vec4(1),
		const glm::vec4& a_specular = glm::vec4(1),
		const glm::mat4& a_shadowMatrix = glm::mat4(1));

	// Initialises BaseCamera
	void InitialiseFlyCamera(c_float a_minSpeed,
		c_float a_maxSpeed, c_float a_rotationSpeed,
		glm::vec3 a_position, glm::vec3 a_lookAt);

	// Program Shader ID
	uint* m_pMainProgramID;

	// window app to allow visuals
	GLFWwindow* m_pWindow;
	// View Camera
	BaseCamera* m_pBaseCamera;
	// AntTweak GUI intigration
	AntTweak* m_pAntTweakGUI;
	// SkyBox viewer
	SkyBox* m_pSkyBox;
	// Sun Position
	glm::vec3 m_sunPosition;
private:
	// Initialise internal app variables and functionalities
	void InitialiseAppElements();
	// Holds value for elapsed time
	float m_elapsedTime;
	// Prev DT && FPS && Curr DT
	double m_prevTime, m_FPS, m_deltaTime;
	// Size of window
	int m_width, m_height;
	// Title of window
	c_charp m_title;
	// Background colour
	glm::vec4 m_backColour;
};