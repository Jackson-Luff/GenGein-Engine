#pragma once
#include <string>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

struct GLFWwindow;
class BaseCamera;
class AntTweak;
class SkyBox;

using namespace glm;

////
// Author: Jackson Luff
// Name: BaseApp
// Type: class (standard)
// Parent: None
// Description:
// * BaseApp contains all the nasty openGL
// * away from the user to allow a 'smooth' - LOL flow 
// * throughout this framework.
////
class BaseApp
{
public:
	// Constructor
	BaseApp();
	BaseApp(const int& a_width, const int& a_height, const char* a_title);
	// Deconstructor
	~BaseApp();

	// Initialise GLFW window
	bool InitialiseGL();
	
	// Getter for FPS count
	inline const double& GetFPS() const
		{ return m_FPS; }
	// Getter for elapsed gametime 
	inline const float& GetElapsedTime() const 
		{ return m_elapsedTime; }
	// Getter for Delta Time
	inline const double& GetDeltaTime() const 
		{ return m_deltaTime; }
	// Getter for MousePosition
	inline const dvec2 GetMousePosition() const
	{
		double x = 0, y = 0;
		glfwGetCursorPos(m_pWindow, &x, &y);
		return dvec2(x, y);
	}

	// Initialised content 
	virtual void StartUp() = 0;
	// Deconstructed content
	virtual void ShutDown() = 0;
	// Update loop
	virtual void Update( const double& a_dt ) = 0;
	// Render to GUI
	virtual void Render() = 0;
	// Run the application
	void Run();
	// Render SkyBox
	void RenderSkyBox();

protected:
	// REMOVE:Temp hotkeys for debugging
	void DebugControls();
	// Calculate deltaTime and elapsedTiming
	void CalculateTiming();
	
	// Return the width and/or height of the window
	const int& GetWidth() const 
		{ return m_width; }
	const int& GetHeight() const
		{ return m_height; }
	
	// Return the size of the window
	inline const ivec2 GetWindowSize() const 
		{ return ivec2(m_width, m_height); }
	inline void SetWindowSize(const int& a_newWidth, const int& a_newHeight)
		{ glfwSetWindowSize(m_pWindow, a_newWidth, a_newHeight); }
	
	// Sends camera data to ALL shaderprograms
	void ApplyCameraUniformSetup();

	// Set-up basic camera data
	void ApplyLightingSetup(
		const vec3& a_ambient,
		const vec3& m_sunPosition,
		const float& a_strtLightingHeight);

	// Initialises Fly Camera
	void InitialiseFlyCamera(const float& a_minSpeed,
		const float& a_maxSpeed,
		const float& a_rotationSpeed,
		const vec3& a_position, 
		const vec3& a_lookAt);

	// Initialise Pan-Zoom Camera
	//void InitialisePanZoomCamera(
	//	const float32_t& a_moveSpeed,
	//	const float32_t& a_rotSpeed,
	//	const f32vec3& a_position,
	//	const f32vec3& a_lookAt);

	// Program Shader ID
	unsigned int* m_pMainProgramID;

	// window app to allow visuals
	GLFWwindow* m_pWindow;
	// View Camera
	BaseCamera* m_pBaseCamera;
	// AntTweak GUI intigration
	AntTweak* m_pAntTweakGUI;
	// SkyBox viewer
	SkyBox* m_pSkyBox;
	// Sun Position
	vec3 m_sunPosition;
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
	const char* m_title;
	// Background colour
	vec4 m_backColour;
};