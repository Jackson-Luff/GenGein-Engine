#pragma once
#include <map>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

using namespace glm;
using const_pChar = const char*;

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
	enum class ShaderType : uint32_t
	{
		VERT_SHADER = GL_VERTEX_SHADER,
		FRAG_SHADER = GL_FRAGMENT_SHADER,
		GEOM_SHADER = GL_GEOMETRY_SHADER,
		TESSC_SHADER = GL_TESS_CONTROL_SHADER,
		TESSE_SHADER = GL_TESS_EVALUATION_SHADER,
	};

	struct DirectoryData
	{
		const_pChar vertDir;
		const_pChar pixeDir;
		const_pChar geomDir;
		const_pChar tesCDir;
		const_pChar tesEDir;
	};

	// Deconstructor
	~ShaderHandler();

	//Getter for the shader program
	static unsigned int& GetShader(const_pChar a_shaderName);
	// Create a GL program with inputted vert and frag shaders
	static const uint32_t LoadShaderProgram(const_pChar a_shaderName,
		const_pChar a_vertexShader, const_pChar a_pixelShader,
		const_pChar a_geometryShader = nullptr, const_pChar a_tessCntrlShader = nullptr,
		const_pChar a_tessEvalShader = nullptr,
		bool checkForExists = true);
	
	// Initialise Shader Content
	static const void CreateShader(const_pChar a_shaderDir, const ShaderType& a_type, uint32_t& a_IDContainer);

	static const void SetUpCameraUniforms(
		const f32mat4& a_camProjMat,
		const f32mat4& a_camViewMat,
		const f32mat4& a_camWorldMat);

	static const void SetUpLightingUniforms(
		const f32vec3& a_ambientLight,
		const f32vec3& a_SunPos,
		const float32& a_strtLightingHeight,
		const float32& a_elapsedTime);

	static const void ReloadAllPrograms();
	static const void UnloadAllPrograms();
private:

	// Returns if the programID was found in the map
	static const bool DoesShaderExist(const_pChar& a_shaderName);
	// Determines the success of the shader
	static const bool CheckShaderStatus(const uint32_t& a_shaderName);
	// Determines the success of the program
	static const bool CheckProgramStatus(const uint32_t& prog);
	// Reads the shader based on the directory 
	static const_pChar ReadShaderCode(const_pChar a_filePath);

	static std::map< unsigned int, DirectoryData> m_directoryMap;
	static std::map< const_pChar, unsigned int > m_programMap;
};