#pragma once
#include <map>
#include <GLFW\glfw3.h>
#include <AntTweakBar.h>
#include <glm\glm.hpp>

typedef unsigned int uint;
typedef const char* c_pChar;

class AntTweak
{
public:
	AntTweak();
	~AntTweak();

	void Initialise( GLFWwindow* a_prog, const uint a_scrnWidth, const uint a_scrnHeight );

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
		static void OnMouseButton( GLFWwindow*, int b, int a, int m ) {
			TwEventMouseButtonGLFW(b,a);
		}
		static void OnMousePosition( GLFWwindow*, double x, double y ) {
			TwEventMousePosGLFW((int)x, (int)y);
		}
		static void OnMouseScroll( GLFWwindow*, double x, double y ) {
			TwEventMouseWheelGLFW((int)y);
		}
		static void OnKey( GLFWwindow*, int k, int s, int a, int m ) {
			TwEventKeyGLFW(k, a);
		}
		static void OnChar( GLFWwindow*, uint c ) {
			TwEventCharGLFW(c, 1); // returns pressed
		}
		static void OnWindowResize(GLFWwindow*, int w, int h) {
			TwWindowSize(w,h);
			glViewport(0,0,w,h);
		}
	};
};

