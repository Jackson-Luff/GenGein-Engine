#pragma once
#include <vector>
#include <string>
#include <glm\glm.hpp>

using const_str =  const std::string;
using const_pChar = const char*;

struct FBXMaterial;

struct sTexture
{
	uint32_t ID;
	unsigned int* programID;
	int32_t textureUniLoc;
};

class TextureHandler
{
public:
	
	// Add a material based on desired name and directory
	static const sTexture LoadTexture(const_pChar a_prog, const_pChar a_name, const_str& a_dir);
	static const sTexture LoadPerlin( const_pChar a_prog, const_pChar a_name, const uint32_t& a_dim);
	static const sTexture LoadCubeMap(const_pChar a_prog, const_pChar a_name, std::vector<const_str>& a_faces);
	static const uint32_t LoadFrameBuffer(const_pChar a_prog, const_pChar a_name, const int32_t& a_width, const int32_t& a_height);
	static const void	  LoadFBXTexture(const_pChar a_prog, const FBXMaterial* a_texture);
	// Get the texture by name
	static const sTexture GetTexture(const_pChar a_name);
	static const int32_t  GetTextureIndex(const_pChar);
	// Remove a material by name
	static const void UnloadTexture(const_pChar a_name);
	static const void UnloadAllTextures();
	// Rendering the textures
	static const void RenderAllTextures();
private:
	// Check if texture exists
	static const bool DoesTextureExist(const_pChar a_name);

	static std::vector<std::pair<const_pChar, sTexture>> m_textureMap;
};