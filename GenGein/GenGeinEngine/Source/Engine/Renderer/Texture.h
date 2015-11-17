#pragma once

class Texture
{
public:
	Texture();
	~Texture();

	void Setup(const char* a_shader, const char* a_name, const char* a_directory, unsigned int a_textureSlot, unsigned int a_textureType = -1);
	void Shutdown();

	void Render();
private:
	unsigned int* m_programID;
	unsigned int m_textureBuffID;
	unsigned int m_textureSlotID;
	unsigned int m_textureType;
	unsigned int m_textureUniLoc;
};