#include <string>
#include <AntTweakBar.h>
#include <GLFW\glfw3.h>
#include "AntTweak.h"

AntTweak::AntTweak()
{}

AntTweak::~AntTweak()
{}

void AntTweak::Initialise(GLFWwindow* a_window, const uint32_t& a_scrnWidth, const uint32_t& a_scrnHeight)
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
	glfwSetWindowSizeCallback(a_window, TweakBarFunc::OnWindowResize);
};

void AntTweak::ShutDown()
{
	TwDeleteAllBars();
	TwTerminate();
}

void AntTweak::AddTweaker( c_pChar a_name )
{
	// NOTE: MAY POINT TO THE ADDRESS AND BREAK
	std::string description = "group= Tweaker";
	TwBar* bar = TwNewBar(a_name);
}

void AntTweak::AddVarRO( c_pChar a_Tweakname, c_pChar a_groupName, c_pChar a_varName, TwType a_type, void* a_addressOfData)
{
	// Get the first word of the variable name
	// for the description
	std::string groupName = a_groupName;
	std::string description = "group = " + groupName;
	TwAddVarRO( TwGetBarByName(a_Tweakname), a_varName,
		a_type, a_addressOfData, description.c_str());
}

void AntTweak::AddVarRW( c_pChar a_Tweakname, c_pChar a_groupName, c_pChar a_varName, TwType a_type, void* a_addressOfData)
{
	// Get the first word of the variable name
	// for the description
	std::string groupName = a_groupName;
	std::string description = "group =" + groupName;
	TwAddVarRW( TwGetBarByName(a_Tweakname), a_varName,
		a_type, a_addressOfData, description.c_str());
}

void AntTweak::RemoveVar( c_pChar a_TweakName, c_pChar a_varName )
{
	TwRemoveVar(TwGetBarByName(a_TweakName), a_varName);
}

void AntTweak::RemoveAllVars( c_pChar a_TweakName )
{
	TwRemoveAllVars( TwGetBarByName(a_TweakName) );
}

void AntTweak::Draw()
{
	TwDraw();
}