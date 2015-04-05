#pragma once
#include <vector>
#include <map>
#include <string>

typedef unsigned int		uint;
typedef const unsigned int	c_uint;
typedef const char*			c_charp;
typedef const std::string c_str;

struct sTexture
{
	uint ID, programID;
	int textureUniLoc;
};

class TextureHandler
{
public:

	// Deconstructor
	~TextureHandler();
	
	// Add a material based on desired name and directory
	static sTexture LoadTexture(c_uint& a_prog, c_charp a_name, c_str a_dir);
	static sTexture LoadPerlin(c_uint& a_prog, c_charp a_name, c_uint a_dim);
	static sTexture LoadCubeMap(uint a_prog, c_charp a_name, std::vector<c_str> a_faces);
	// Get the texture by name
	static inline sTexture GetTexture(c_charp a_name);
	// Remove a material by name
	static void UnloadTexture(c_charp a_name);
	// Rendering the textures
	static void RenderAllTextures();
	static void RenderAllCubeMaps();

	static void UpdatePerlin(c_charp a_name, c_uint a_dim, const float& a_seed);
private:
	// Check if texture exists
	static bool DoesTextureExist(c_charp a_name);

	static std::map < c_charp, sTexture > m_textureMap;
};