#include "Engine\Core\ShaderHandler.h"
#include "Projects\TIANW\Terrain\SegmentedGrid.h"
#include "Projects\TIANW\Terrain\GradientNoise.h"
#include "WaterPlane.h"


WaterPlane::WaterPlane()
{
}


WaterPlane::~WaterPlane()
{
	delete m_pWaterPlane;
}

void WaterPlane::SetUp(unsigned int a_dims, float a_dimScale)
{
	ShaderHandler::LoadShaderProgram("Water",
		"Data/Shaders/TIANW/Water.vert",
		"Data/Shaders/TIANW/Water.frag");
	m_pWaterProgramID = &ShaderHandler::GetShader("Water");

	m_pWaterPlane = new SegmentedGrid();
	m_pWaterPlane->GenSegGrid(a_dims, a_dimScale);
	m_pWaterPlane->ApplyMapToBuffers();
}

void WaterPlane::Render(unsigned int a_skyBox)
{
	m_pWaterPlane->Render(m_pWaterProgramID);
}