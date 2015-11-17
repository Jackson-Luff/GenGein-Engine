#include "Engine\Core\ShaderHandler.h"

#include "Engine\Renderer\Texture.h"
#include "SegmentedGrid.h"
#include "GradientNoise.h"
#include "Terrain.h"

Terrain::Terrain()
{}

Terrain::~Terrain()
{}

void Terrain::SetUp(unsigned int a_dims, float a_dimScale)
{
	m_dimensions = a_dims;

	ShaderHandler::LoadShaderProgram("Terrain",
		"Data/Shaders/TIANW/Terrain.vert",
		"Data/Shaders/TIANW/Terrain.frag");
	m_pTerrainProgramID = &ShaderHandler::GetShader("Terrain");

	m_pTerrainGrid = new SegmentedGrid();
	m_pTerrainGrid->GenPerlinWithNormals(a_dims, a_dimScale);
	m_pTerrainGrid->ApplyMapToBuffers();

	// Texture slot 1 (bumpy map)
	//m_pBumpyPerlinTexture = new GradientNoise();
	//m_pBumpyPerlinTexture->GeneratePerlinTexture("Terrain", GL_TEXTURE1, a_dims, 6, 1.0f, 0.3f, 2.5f, 1, "bumpyPerlinTexture");
	//// Texture slot 2 (smoothed map)
	//m_pSmoothPerlinTexture = new GradientNoise();
	//m_pSmoothPerlinTexture->GeneratePerlinTexture("Terrain", GL_TEXTURE2, a_dims, 6, 1.0f, 0.1f, 2.0f, 1, "smoothPerlinTexture");
	//
	//// Texture slot 1
	//m_textureSand = new Texture();
	//m_textureSand->Setup("Terrain", "SandTexture",
	//	"Data/Textures/Terrain/Sand_Average.jpg", GL_TEXTURE_2D);
	//
	//// Texture slot 2 
	//m_textureDirt = new Texture();
	//m_textureDirt->Setup("Terrain", "DirtTexture",
	//	"Data/Textures/Terrain/Dirt.jpg", GL_TEXTURE_2D);

	// Texture slot 1
	m_pDiffuseGrass = new Texture();
	m_pDiffuseGrass->Setup("Terrain", "grassDiffuse",
		"Data/Textures/HDTextures/156.JPG", GL_TEXTURE2, GL_TEXTURE_2D);

	// Texture slot 2
	m_pNormalGrass = new Texture();
	m_pNormalGrass->Setup("Terrain", "grassNormal",
		"Data/Textures/HDTextures/156_norm.JPG", GL_TEXTURE3, GL_TEXTURE_2D);

	// Texture slot 3
	m_pDiffuseStone = new Texture();
	m_pDiffuseStone->Setup("Terrain", "stoneDiffuse",
		"Data/Textures/HDTextures/151.JPG", GL_TEXTURE4, GL_TEXTURE_2D);

	// Texture slot 4
	m_pNormalStone = new Texture();
	m_pNormalStone->Setup("Terrain", "stoneNormal",
		"Data/Textures/HDTextures/151_norm.JPG", GL_TEXTURE5, GL_TEXTURE_2D);
}

void Terrain::CleanUp()
{
	m_pDiffuseGrass->Shutdown();
	delete m_pDiffuseGrass;
	m_pNormalGrass->Shutdown();
	delete m_pNormalGrass;
	
	m_pDiffuseStone->Shutdown();
	delete m_pDiffuseStone;
	m_pNormalStone->Shutdown();
	delete m_pNormalStone;
	
	//delete m_pBumpyPerlinTexture;
	//delete m_pSmoothPerlinTexture;
	
	delete m_pTerrainGrid;
}

void Terrain::Render()
{
	//m_pBumpyPerlinTexture->Render();
	//m_pSmoothPerlinTexture->Render();

	m_pDiffuseGrass->Render();
	m_pNormalGrass->Render();
	m_pDiffuseStone->Render();
	m_pNormalStone->Render();
	
	m_pTerrainGrid->Render(m_pTerrainProgramID);
}

// Private Func's:

