#include <iostream>
#include <map>
#include "InputManager.h"

std::map<c_charp, uint>InputManager::m_keysWereDown = {};
std::map<c_charp, uint>InputManager::m_keysAreDown = {};

bool InputManager::WasKeyDown(c_charp a_key)
{
	if (GetKey(a_key) == PRESS)
		return true;

	return false;
}

bool InputManager::IsKeyDown(c_charp a_key)
{
	if (GetKey(a_key) == PRESS)
		return true;

	return false;
}

bool InputManager::IsKeyUp(c_charp a_key)
{
	if (GetKey(a_key) == PRESS)
		return true;

	return false;
}

bool InputManager::GetKey(c_charp a_key)
{
	if (m_keysAreDown[a_key] != NULL)
		return true;

	return false;
}