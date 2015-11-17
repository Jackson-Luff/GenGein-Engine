#pragma once

class PhysScene;

class PhysHandle
{
public:
	PhysHandle();
	~PhysHandle();

	// helper functions to keep StartUp clean
	void BuildWalling();
	void BuildPhysGeo();

	void StartUp();
	void ShutDown();
	void Update(double a_dt);
	void Render();
private:
	PhysScene* m_scene;
};