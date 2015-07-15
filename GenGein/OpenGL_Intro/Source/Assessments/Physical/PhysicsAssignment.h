#pragma once
#include "Engine\Core\BaseApp.h"

class PhysXHandle;
class PhysicsHandle;

////
// Author: Jackson Luff
// Name: PhysicsAssignment
// Type: class (standard)
// Parent: none
// Description:
// * 
// * 
// * 
////
class PhysicsAssignment : public BaseApp
{
public:
	PhysicsAssignment();
	PhysicsAssignment(const int a_width, const int a_hieght, const char* a_title);
	~PhysicsAssignment();

	// Initialise mem var's
	virtual void StartUp();
	// Destroy things
	virtual void ShutDown();

	// Update loop
	virtual void Update(const double& a_dt);
	// Render things to screen
	virtual void Render();
private:
	PhysXHandle* m_physXHandle;
	PhysicsHandle* m_physicsHandle;
};