#include "Engine\Core\ShaderHandler.h"
#include "Engine\Cameras\BaseCamera.h"
#include "Engine\Objects\FBXModel.h"
#include "Engine\Input\InputHandle.h"
#include "PhysHandle.h"
#include "PhysXHandle.h"

#include "PhysicsAssignment.h"

PhysicsAssignment::PhysicsAssignment() : BaseApp()
{}

PhysicsAssignment::PhysicsAssignment(const int a_width, const int a_height, const char* a_title)
	: BaseApp(a_width, a_height, a_title)
{}

PhysicsAssignment::~PhysicsAssignment()
{
	delete m_pPhysicsHandle;
	delete m_pPhysXHandle;
}

void PhysicsAssignment::StartUp()
{
	BaseApp::StartUp();

	ShaderHandler::LoadShaderProgram("Divider",
		"Data/Shaders/Physics/Divider.vert",
		"Data/Shaders/Physics/Divider.frag");
	
	ShaderHandler::LoadShaderProgram("Phys",
		"Data/Shaders/Physics/Phys.vert",
		"Data/Shaders/Physics/Phys.frag");

	ShaderHandler::LoadShaderProgram("PhysX",
		"Data/Shaders/Physics/PhysX.vert",
		"Data/Shaders/Physics/PhysX.frag");

	ShaderHandler::LoadShaderProgram("Fluid",
		"Data/Shaders/Physics/Fluid.vert",
		"Data/Shaders/Physics/Fluid.frag");

	//Initialise camera
	InitialiseFlyCamera(15.0f, 20.0f, 0.5f,
		glm::vec3(0, 85, 197), glm::vec3(0));

	Input::Camera::SetCamera(m_pBaseCamera);

	m_pDivWall = new FBXModel();
	m_pDivWall->LoadFBX("Divider", "Data/Models/Cube.fbx", FBXFile::UNITS_CENTIMETER);

	m_pPhysicsHandle = new PhysHandle();
	m_pPhysicsHandle->StartUp();

	m_pPhysXHandle = new PhysXHandle();
	m_pPhysXHandle->StartUp();

	Input::Time::SetElapsedTime(GetElapsedTime());
	Input::Time::SetDeltaTime(GetDeltaTime());
}

void PhysicsAssignment::ShutDown()
{
	m_pPhysicsHandle->ShutDown();
	m_pPhysXHandle->ShutDown();
}

bool isWithinBox(glm::vec3 a_pos, glm::vec3 a_size, glm::vec3 a_compareMe)
{
	if (a_compareMe.x > a_pos.x - a_size.x && a_compareMe.x < a_pos.x + a_size.x &&
		a_compareMe.y > a_pos.y - a_size.y && a_compareMe.y < a_pos.y + a_size.y &&
		a_compareMe.z > a_pos.z - a_size.z && a_compareMe.z < a_pos.z + a_size.x)\
		return true;

	return false;
}

void PhysicsAssignment::Update(const double& a_dt)
{
	BaseApp::Update(a_dt);

	glm::vec3 camPos = Input::Camera::GetCamPos();
	
	if (isWithinBox(glm::vec3(-55, 0, 0), glm::vec3(55), camPos))
		m_pPhysicsHandle->Update(1/ 60.0f);
	else if (isWithinBox(glm::vec3(55, 0, 0), glm::vec3(55), camPos))
		m_pPhysXHandle->Update(1 / 60.0f);
}

void PhysicsAssignment::Render()
{
	BaseApp::Render();

	//ApplyLightingSetup(glm::vec3(0.25f),
	//	glm::vec3(10), 1);

	m_pPhysicsHandle->Render();
	m_pPhysXHandle->Render();

	glm::vec3 p(0, 10.0f, -20);
	glm::vec3 s(0.5f, 20.0f, 35);
	
	m_pDivWall->SetLocalTransform(glm::translate(p) * glm::scale(s));
	m_pDivWall->Render();
}