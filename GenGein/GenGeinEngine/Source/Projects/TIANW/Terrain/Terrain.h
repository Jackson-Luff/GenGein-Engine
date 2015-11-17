#pragma once

class Texture;
class SegmentedGrid;
class GradientNoise;

////
// Author: Jackson Luff
// Name: Terrain
// Type: class (standard)
// Parent: None
// Description:
// * Terrain is a class that has everything to do
// * with the environment side of things in the 
// * application 'TIANW'.
// *
// * Various features shown in this class are 
// * broken up into other classes to seperate 
// * the logic from the visuals.
// * Features:
// * - Height Mapping with a generated perlin-noise map.
// * 
// * Future-Features:
// *  - Various implementations of textures.
// *	(e.g. Rock texture based on vertex normals)
// *  - Mip-mapped texture implementation.
// *  - Dynamic generation with several noise maps.
////
class Terrain
{
public:
	Terrain();
	~Terrain();

	// Initialise environment
	void SetUp(unsigned int a_dims, float a_dimScale);
	// Shutdown environment
	void CleanUp();

	void Render();

	// Returns dimensions of grid
	unsigned int GetDimensions() 
		{ return m_dimensions; };

private:
	//Texture* m_pTextureSand;
	//Texture* m_pTextureDirt;
	Texture* m_pDiffuseGrass;
	Texture* m_pNormalGrass;
	Texture* m_pDiffuseStone;
	Texture* m_pNormalStone;

	// Generates a segmented grid 
	SegmentedGrid* m_pTerrainGrid;
	// Generates height map texture
	GradientNoise* m_pBumpyPerlin;
	GradientNoise* m_pSmoothPerlin;

	
	unsigned int* m_pTerrainProgramID;
	unsigned int m_dimensions;
};