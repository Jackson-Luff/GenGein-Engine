#include <GLFW\glfw3.h>

#include "InputHandle.h"

using Input::Keyboard;
using Input::Cursor;
using Input::Window;

// Keyboard

bool Keyboard::isKeyUp(const int a_key)
{
	if (!glfwGetKey(glfwGetCurrentContext(), (int)a_key))
		return true;

	return false;
}

bool Keyboard::isKeyDown(const int a_key)
{
	if (glfwGetKey(glfwGetCurrentContext(), (int)a_key))
		return true;

	return false;
}

bool Keyboard::isKeyRelease(const int a_key)
{
	return false;
}

void Keyboard::RunDebugControls()
{
	if (isKeyDown(KEY_GRAVE_ACCENT))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (isKeyDown(KEY_1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (isKeyDown(KEY_2))
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
// Cursor

glm::dvec2 Cursor::m_oldCursorPos = glm::dvec2(0);

bool Cursor::isLeftButtonDown()
{
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_1))
		return true;

	return false;
}

bool Cursor::isRightButtonDown()
{
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_2))
		return true;

	return false;
}

glm::dvec2 Cursor::GetCursorPos()
{
	double x, y;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);

	m_oldCursorPos = glm::dvec2(x, y);

	return glm::dvec2(x, y);
}

glm::dvec2 Cursor::GetOldCursorPos()
{
	return m_oldCursorPos;
}

void Cursor::SetCursorPos(const double a_X, const double a_Y)
{
	glfwSetCursorPos(glfwGetCurrentContext(), a_X, a_Y);
}

// Window

glm::ivec2 Window::GetWindowSize()
{
	int width, height;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	return glm::ivec2(width, height);
}