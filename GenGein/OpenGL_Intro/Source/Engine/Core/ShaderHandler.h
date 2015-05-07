#pragma once
#include <map>
#include <glm\glm.hpp>

typedef unsigned int	   uint;
typedef const unsigned int c_uint;
typedef const char*		   c_pChar;

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
		c_pChar vertDir;
		c_pChar pixeDir;
		c_pChar geomDir;
		c_pChar tesCDir;
		c_pChar tesEDir;
	};

	// Deconstructor
	~ShaderHandler();

	//Getter for the shader program
	static uint& GetShader(c_pChar a_shaderName);
	// Create a GL program with inputted vert and frag shaders
	static uint LoadShaderProgram( c_pChar a_shaderName,
		c_pChar a_vertexShader, c_pChar a_pixelShader,
		c_pChar a_geometryShader = nullptr, c_pChar a_tessCntrlShader = nullptr,
		c_pChar a_tessEvalShader = nullptr,
		bool checkForExists = true);
	
	// Initialise Shader Content
	static uint CreateShader(c_pChar a_shaderDir, c_uint a_type);

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

	static const void UnloadAllPrograms();
private:

	// Returns if the programID was found in the map
	static const bool DoesShaderExist(c_pChar& a_shaderName);
	// Determines the success of the shader
	static const bool CheckShaderStatus(c_uint& a_shaderName);
	// Determines the success of the program
	static const bool CheckProgramStatus( c_uint& prog );
	// Reads the shader based on the directory 
	static c_pChar ReadShaderCode( c_pChar a_filePath);

	static std::map< uint, DirectoryData> m_directoryMap;
	static std::map< c_pChar, uint > m_programMap;
};