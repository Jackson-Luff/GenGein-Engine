#pragma once
#include <vector>
#include <map>
#include <string>

typedef unsigned int		uint;
typedef const unsigned int	c_uint;
typedef const char*			c_pChar;
typedef const std::string c_str;

struct FBXMaterial;

struct sTexture
{
	uint ID;
	uint* programID;
	int textureUniLoc;
};

class TextureHandler
{
public:

	// Deconstructor
	~TextureHandler();
	
	// Add a material based on desired name and directory
	static sTexture LoadTexture(c_pChar a_prog, c_pChar a_name, c_str a_dir);
	static sTexture LoadPerlin(c_pChar a_prog, c_pChar a_name, c_uint a_dim);
	static sTexture LoadCubeMap(c_pChar a_prog, c_pChar a_name, std::vector<c_str> a_faces);
	static uint		LoadFrameBuffer(c_pChar a_prog, c_pChar a_name, int a_width, int a_height);
	static void		LoadFBXTexture(c_pChar a_prog, const FBXMaterial* a_texture);
	// Get the texture by name
	static inline sTexture GetTexture(c_pChar a_name);
	// Remove a material by name
	static void UnloadTexture(c_pChar a_name);
	static void UnloadAllTextures();
	// Rendering the textures
	static void RenderAllTextures();
private:
	// Check if texture exists
	static bool DoesTextureExist(c_pChar a_name);

	static std::map < c_pChar, sTexture > m_textureMap;
};