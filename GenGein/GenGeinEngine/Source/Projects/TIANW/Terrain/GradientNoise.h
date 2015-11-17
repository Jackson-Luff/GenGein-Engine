#pragma once

////
// Author: Jackson Luff
// Name: GradientNoise
// Type: multiuse-class (standard)
// Parent: None
// Description:
// * GradientNoise is a general class that 
// * has several gradient map uses. 
// * E.g. perlin noise and diamond square
// *	can be generated from this map.
// *	(A more cleaner use would be to have
// *	this as a base for inheriting instead.)
// * 
// * Features:
// * Generates Perlin-Noise		(Data Array &| Texture)
// * Generates Diamond Square	(Data Array &| Texture)
// * 
////
class GradientNoise
{
public:
	GradientNoise();
	~GradientNoise();

	// Generated perlin map and binds to a texture buffer.
	void GeneratePerlinTexture(const char* a_shaderName,
		unsigned int a_textureSlot, unsigned int a_dims,
		unsigned int a_octaves, float a_amp, float a_persis,
		float a_seed, const char* a_name);

	// Renders generated texture
	void Render();

private:
	// Creates a constant array of perlin noise data
	// TODO: allow varied input for key perlin-noise variables (e.g. seed)
	float* GeneratePerlinNoiseMap(uint a_dims, uint a_octaves,
		float a_amp, float a_persist, float a_seed);

	// Binds gradient data from map to a texture buffer
	void BindMapToTextureBuffers(const char* a_name, unsigned int a_dims, float* a_gradientData);

	unsigned int* m_programID;
	unsigned int m_textureSlotID;
	unsigned int m_textureMapID;
	unsigned int m_textureUniLoc;
};

