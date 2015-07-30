#pragma once

class PhysScene;

class PhysHandle
{
public:
	PhysHandle();
	~PhysHandle();

	void StartUp();
	void ShutDown();
	void Update(const double a_dt);
	void Render();
private:
	PhysScene* m_scene;
};