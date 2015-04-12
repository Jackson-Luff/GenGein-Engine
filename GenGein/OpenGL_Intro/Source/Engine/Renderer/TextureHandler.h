#pragma once
#include <vector>
#include <map>
#include <string>

typedef unsigned int		uint;
typedef const unsigned int	c_uint;
typedef const char*			c_charp;
typedef const std::string c_str;

struct FBXTexture;

struct sTexture
{
	uint ID;
	c_uint* programID;
	int textureUniLoc;
};

class TextureHandler
{
public:

	// Deconstructor
	~TextureHandler();
	
	// Add a material based on desired name and directory
	static sTexture LoadTexture(c_uint* a_prog, c_charp a_name, c_str a_dir);
	static sTexture LoadFBXTexture(c_uint a_prog, const FBXTexture* a_data);
	static sTexture LoadPerlin(c_uint* a_prog, c_charp a_name, c_uint a_dim);
	static sTexture LoadCubeMap(c_uint* a_prog, c_charp a_name, std::vector<c_str> a_faces);
	// Get the texture by name
	static inline sTexture GetTexture(c_charp a_name);
	// Remove a material by name
	static void UnloadTexture(c_charp a_name);
	// Rendering the textures
	static void RenderAllTextures();
	static void RenderAllCubeMaps();
private:
	// Check if texture exists
	static bool DoesTextureExist(c_charp a_name);

	static std::map < c_charp, sTexture > m_textureMap;
};