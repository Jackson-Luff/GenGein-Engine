#include <string>
#include <AntTweakBar.h>
#include <GLFW\glfw3.h>
#include "AntTweak.h"

AntTweak::AntTweak()
{}

AntTweak::~AntTweak()
{
	TwDeleteAllBars();
	TwTerminate();
}

void AntTweak::Initialise( GLFWwindow* a_window, const uint a_scrnWidth, const uint a_scrnHeight )
{
	// Initialise Tweaker
	if(!TwInit(TW_OPENGL_CORE, nullptr))
		return;
	
	// Apply Screen Resoultion
	if(!TwWindowSize(a_scrnWidth, a_scrnHeight))
		return;

	// Set all the input settings for GL
	glfwSetMouseButtonCallback(a_window, TweakBarFunc::OnMouseButton);
	glfwSetCursorPosCallback(a_window, TweakBarFunc::OnMousePosition);
	glfwSetScrollCallback(a_window, TweakBarFunc::OnMouseScroll);
	glfwSetKeyCallback(a_window, TweakBarFunc::OnKey);
	glfwSetCharCallback(a_window, TweakBarFunc::OnChar);
	glfwSetWindowSizeCallback(a_window, TweakBarFunc::OnWindowResize);};

void AntTweak::AddTweaker( c_charp a_name, c_charp a_colourName, glm::vec4& a_colour )
{
	// NOTE: MAY POINT TO THE ADDRESS AND BREAK
	std::string description = "group= Tweaker";
	TwBar* bar = TwNewBar(a_name);
	
	AddVarRW(a_name, a_colourName, TW_TYPE_COLOR4F, (void*)&a_colour);
}

void AntTweak::AddVarRO( c_charp a_Tweakname, c_charp a_varName, TwType a_type, void* a_addressOfData)
{
	// Get the first word of the variable name
	// for the description
	std::string c;
	
	for(int i = 0; !(isspace(a_varName[i])); i++)
		c += a_varName[i];

	std::string description = "group = " + c;

	TwAddVarRO( TwGetBarByName(a_Tweakname), a_varName,
		a_type, a_addressOfData, description.c_str());
}

void AntTweak::AddVarRW( c_charp a_Tweakname, c_charp a_varName, TwType a_type, void* a_addressOfData)
{
	// Get the first word of the variable name
	// for the description
	std::string c;
	
	for(int i = 0; !(isspace(a_varName[i])); i++)
		c += a_varName[i];

	std::string description = "group = " + c;

	TwAddVarRW( TwGetBarByName(a_Tweakname), a_varName,
		a_type, a_addressOfData, description.c_str());
}

void AntTweak::RemoveVar( c_charp a_TweakName, c_charp a_varName )
{
	TwRemoveVar(TwGetBarByName(a_TweakName), a_varName);
}

void AntTweak::RemoveAllVars( c_charp a_TweakName )
{
	TwRemoveAllVars( TwGetBarByName(a_TweakName) );
}

void AntTweak::Draw()
{
	TwDraw();
}