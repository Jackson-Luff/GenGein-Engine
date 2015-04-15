#pragma once
#include <map>
#include <glm\glm.hpp>

typedef unsigned int	   uint;
typedef const unsigned int c_uint;
typedef const char*		   c_charp;

////
// Author: Jackson Luff
// Name: ShaderLoader
// Type: class (standard)
// Parent: None
// Description:
// * ShaderLoader loads and handles shader 
// * formatted files and applies accordingly.
////
class ShaderHandler
{
public:
	struct DirectoryData
	{
		c_charp vertDir;
		c_charp pixeDir;
		c_charp geomDir;
		c_charp tesCDir;
		c_charp tesEDir;
	};

	// Deconstructor
	~ShaderHandler();

	//Getter for the shader program
	static uint& GetShader(c_charp a_shaderName);
	// Create a GL program with inputted vert and frag shaders
	static uint LoadShaderProgram( c_charp a_shaderName,
		c_charp a_vertexShader, c_charp a_pixelShader,
		c_charp a_geometryShader = nullptr, c_charp a_tessCntrlShader = nullptr,
		c_charp a_tessEvalShader = nullptr,
		bool checkForExists = true);
	
	// Initialise Shader Content
	static uint CreateShader(c_charp a_shaderDir, c_uint a_type);

	static const void SetUpCameraUniforms(
		const glm::mat4& a_camProjMat,
		const glm::mat4& a_camViewMat,
		const glm::mat4& a_camWorldMat);

	static const void SetUpLightingUniforms(
		const glm::vec3& a_ambientLight,
		const glm::vec3& a_SunPos,
		const float& a_strtLightingHeight,
		const float& a_elapsedTime);

	static const void ReloadAllPrograms();
private:

	// Returns if the programID was found in the map
	static const bool DoesShaderExist(c_charp& a_shaderName);
	// Determines the success of the shader
	static const bool CheckShaderStatus(c_uint& a_shaderName);
	// Determines the success of the program
	static const bool CheckProgramStatus( c_uint& prog );
	// Reads the shader based on the directory 
	static c_charp ReadShaderCode( c_charp a_filePath);

	static std::map< uint, DirectoryData> m_directoryMap;
	static std::map< c_charp, uint > m_programMap;
};