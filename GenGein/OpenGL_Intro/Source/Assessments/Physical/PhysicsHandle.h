#pragma once

class PhysScene;

class PhysicsHandle
{
public:
	PhysicsHandle();
	~PhysicsHandle();

	void StartUp();
	
	void ShutDown();

	void Update(const double a_dt);
	
	void Render(const glm::mat4& a_projView);

private:
	PhysScene* m_scene;
};

