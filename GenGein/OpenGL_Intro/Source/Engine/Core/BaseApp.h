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
	BaseApp(const int32_t& a_width, const int32_t& a_height, const char* a_title);
	// Deconstructor
	~BaseApp();

	// Initialise GLFW window
	bool InitialiseGL();
	
	// Getter for FPS count
	inline const double_t& GetFPS() const
		{ return m_FPS; }
	// Getter for elapsed gametime 
	inline const float32_t& GetElapsedTime() const 
		{ return m_elapsedTime; }
	// Getter for Delta Time
	inline const double_t& GetDeltaTime() const 
		{ return m_deltaTime; }
	// Getter for MousePosition
	inline const dvec2 GetMousePosition() const
	{
		double_t x = 0, y = 0;
		glfwGetCursorPos(m_pWindow, &x, &y);
		return dvec2(x, y);
	}

	// Initialised content 
	virtual void StartUp() = 0;
	// Deconstructed content
	virtual void ShutDown() = 0;
	// Update loop
	virtual void Update( const double_t& a_dt ) = 0;
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
	const int32_t& GetWidth() const 
		{ return m_width; }
	const int32_t& GetHeight() const 
		{ return m_height; }
	
	// Return the size of the window
	inline const i32vec2 GetWindowSize() const 
		{ return i32vec2(m_width, m_height); }
	inline void SetWindowSize(const int32_t& a_newWidth, const int32_t& a_newHeight)
		{ glfwSetWindowSize(m_pWindow, a_newWidth, a_newHeight); }
	
	// Sends camera data to ALL shaderprograms
	void ApplyCameraUniformSetup();

	// Set-up basic camera data
	void ApplyLightingSetup(
		const f32vec3& a_ambient,
		const f32vec3& m_sunPosition,
		const float32_t& a_strtLightingHeight);

	// Initialises BaseCamera
	void InitialiseFlyCamera(const float32_t& a_minSpeed,
		const float32_t& a_maxSpeed,
		const float32_t& a_rotationSpeed,
		const f32vec3& a_position, 
		const f32vec3& a_lookAt);

	// Program Shader ID
	uint32_t* m_pMainProgramID;

	// window app to allow visuals
	GLFWwindow* m_pWindow;
	// View Camera
	BaseCamera* m_pBaseCamera;
	// AntTweak GUI intigration
	AntTweak* m_pAntTweakGUI;
	// SkyBox viewer
	SkyBox* m_pSkyBox;
	// Sun Position
	f32vec3 m_sunPosition;
private:
	// Initialise internal app variables and functionalities
	void InitialiseAppElements();
	// Holds value for elapsed time
	float32_t m_elapsedTime;
	// Prev DT && FPS && Curr DT
	double_t m_prevTime, m_FPS, m_deltaTime;
	// Size of window
	int32_t m_width, m_height;
	// Title of window
	const char* m_title;
	// Background colour
	f32vec4 m_backColour;
};