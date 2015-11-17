#pragma once
#include <map>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

using namespace glm;

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
	enum class ShaderType : unsigned int
	{
		VERT_SHADER = GL_VERTEX_SHADER,
		FRAG_SHADER = GL_FRAGMENT_SHADER,
		GEOM_SHADER = GL_GEOMETRY_SHADER,
		TESSC_SHADER = GL_TESS_CONTROL_SHADER,
		TESSE_SHADER = GL_TESS_EVALUATION_SHADER,
	};

	struct DirectoryData
	{
		const char* vertDir;
		const char* pixeDir;
		const char* geomDir;
		const char* tesCDir;
		const char* tesEDir;
	};

	// Deconstructor
	~ShaderHandler();

	//Getter for the shader program
	static unsigned int& GetShader(const char* a_shaderName);
	// Create a GL program with inputted vert and frag shaders
	static const unsigned int LoadShaderProgram(const char* a_shaderName,
		const char* a_vertexShader, const char* a_pixelShader,
		const char* a_geometryShader = nullptr, const char* a_tessCntrlShader = nullptr,
		const char* a_tessEvalShader = nullptr,
		bool checkForExists = true);
	
	// Initialise Shader Content
	static const void CreateShader(const char* a_shaderDir, ShaderType a_type, unsigned int& a_IDContainer);

	static const void SetUpCameraUniforms(
		const mat4& a_camProjMat,
		const mat4& a_camViewMat,
		const mat4& a_camWorldMat);

	static const void SetUpLightingUniforms(
		vec3 a_SunPos);

	static const void SetUpTimerUniforms(
		double a_elapsedTime,
		double a_deltaTime);

	static void ReloadAllPrograms();
	static void UnloadAllPrograms();
private:

	// Returns if the programID was found in the map
	static bool DoesShaderExist(const char* a_shaderName);
	// Determines the success of the shader
	static bool CheckShaderStatus(unsigned int a_shaderName);
	// Determines the success of the program
	static bool CheckProgramStatus(unsigned int prog);
	// Reads the shader based on the directory 
	static const char* ReadShaderCode(const char* a_filePath);

	static std::map< unsigned int, DirectoryData> m_directoryMap;
	static std::map< const char*, unsigned int > m_programMap;
};