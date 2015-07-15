#pragma once
#include <map>
#include <GLFW\glfw3.h>
#include <AntTweakBar.h>
#include <glm\glm.hpp>

typedef const char* c_pChar;

class AntTweak
{
public:
	AntTweak();
	~AntTweak();

	void Initialise(GLFWwindow* a_prog, const uint32_t& a_scrnWidth, const uint32_t& a_scrnHeight);
	void ShutDown();

	void AddTweaker( c_pChar a_tweakName );
	void RemoveTweaker( c_pChar a_tweakName );

	void AddVarRW(c_pChar a_tweakName, c_pChar a_groupName, c_pChar a_varName, TwType a_type, void* a_addressOfData);
	void AddVarRO(c_pChar a_tweakName, c_pChar a_groupName, c_pChar a_varName, TwType a_type, void* a_addressOfData);
	void RemoveVar( c_pChar a_tweakName, c_pChar a_varName );
	void RemoveAllVars( c_pChar a_tweakName );

	void Draw();

private:
	struct TweakBarFunc
	{
	public:
		static void OnMouseButton(GLFWwindow*, int32_t b, int32_t a, int32_t m) {
			TwEventMouseButtonGLFW(b,a);
		}
		static void OnMousePosition( GLFWwindow*, double_t x, double_t y ) {
			TwEventMousePosGLFW((int32_t)x, (int32_t)y);
		}
		static void OnMouseScroll( GLFWwindow*, double_t x, double_t y ) {
			TwEventMouseWheelGLFW((int32_t)y);
		}
		static void OnKey(GLFWwindow*, int32_t k, int32_t s, int32_t a, int32_t m) {
			TwEventKeyGLFW(k, a);
		}
		static void OnChar(GLFWwindow*, uint32_t c) {
			TwEventCharGLFW(c, 1); // returns pressed
		}
		static void OnWindowResize(GLFWwindow*, int32_t w, int32_t h) {
			TwWindowSize(w,h);
			glViewport(0,0,w,h);
		}
	};
};

