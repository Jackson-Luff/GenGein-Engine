#pragma once

class SegmentedGrid;
////
// Author: Jackson Luff
// Name: WaterPlane
// Type: class (standard)
// Parent: None
// Description:
// * This class will control everything in 
// * relation to the water in the application 'TIANW'.
// * 
// * Features included:
// * 
// * Future-Features included:
// * Effective use of 3x perlin noise textures
// *	to create height maps, as well as depth colouring.
// * 
////
class WaterPlane
{
public:
	WaterPlane();
	~WaterPlane();

	void SetUp(unsigned int a_dims, float a_dimScale);

	void Render(unsigned int a_skyBox);
private: 
	SegmentedGrid* m_pWaterPlane;

	unsigned int* m_pWaterProgramID;
};

