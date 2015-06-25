#pragma once
#include "Engine\Core\BaseApp.h"

class PhysScene;

class PhysicsTest01 : public BaseApp
{
public:
	PhysicsTest01();
	PhysicsTest01(const int a_appWidth, const int a_appHeight, const char* a_title);
	~PhysicsTest01();

	void StartUp();
	void ShutDown();

	void Update(const double& a_dt);
	void Render();
private:
	PhysScene* m_scene;
};

