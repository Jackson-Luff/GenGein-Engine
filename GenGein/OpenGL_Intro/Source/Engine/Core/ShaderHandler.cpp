
#include <fstream>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

#include "ShaderHandler.h"

using ShaderMap = std::map<const_pChar, uint>;
using DirectoryMap =  std::map<uint32_t, ShaderHandler::DirectoryData>;
using ShaderType = ShaderHandler::ShaderType;

ShaderMap ShaderHandler::m_programMap = ShaderMap();
DirectoryMap ShaderHandler::m_directoryMap = DirectoryMap();

// Deconstructor
ShaderHandler::~ShaderHandler()
{
}

//Returns shader based on naming
uint& ShaderHandler::GetShader(const_pChar a_shaderName)
{
	if (DoesShaderExist(a_shaderName)) 
		return m_programMap[a_shaderName];

	printf("No shader exists for that name!\n");
}

// Create the program with shader file directories
const uint32_t ShaderHandler::LoadShaderProgram(const_pChar a_shaderName,
	const_pChar a_vertexShader, const_pChar a_pixelShader,
	const_pChar a_geometryShader, const_pChar a_tessCntrlShader,
	const_pChar a_tessEvalShader, bool checkForExists)
{
	if (DoesShaderExist(a_shaderName) && checkForExists)
		return m_programMap[a_shaderName];

	const uint32_t vertShader = 
		CreateShader(a_vertexShader,	ShaderType::VERT_SHADER);
	const uint32_t fragShader =
		CreateShader(a_pixelShader,		ShaderType::FRAG_SHADER);
	const uint32_t geoShader =
		CreateShader(a_geometryShader,	ShaderType::GEOM_SHADER);
	const uint32_t tessCShader =
		CreateShader(a_tessCntrlShader, ShaderType::TESSC_SHADER);
	const uint32_t tessEShader =
		CreateShader(a_tessEvalShader,	ShaderType::TESSE_SHADER);
	
	uint32_t progID;
	// Apply shaders to program
	progID = glCreateProgram();
	glAttachShader(progID, vertShader);
	glAttachShader(progID, fragShader);
	glAttachShader(progID, geoShader);
	glAttachShader(progID, tessCShader);
	glAttachShader(progID, tessEShader);
	glLinkProgram(progID);

	// Check success of program for stable use
	if (!CheckProgramStatus(progID))
		return -1;

	// Deleting shaders/program to
	// minimize memory leaks
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(geoShader);
	glDeleteShader(tessCShader);
	glDeleteShader(tessEShader);

	printf("SUCCESS: Custom Program initialised\n\n");

	m_programMap[a_shaderName] = progID;

	DirectoryData dataDir;
	dataDir.vertDir = a_vertexShader;
	dataDir.pixeDir = a_pixelShader;
	dataDir.geomDir = a_geometryShader;
	dataDir.tesCDir = a_tessCntrlShader;
	dataDir.tesEDir = a_tessEvalShader;
	m_directoryMap[progID] = dataDir;
	
	return m_programMap[a_shaderName];
}

// Initialises the content via valid shader directory
const uint32_t ShaderHandler::CreateShader(const_pChar a_shaderDir, const ShaderType& a_type)
{
	const_pChar src = ReadShaderCode(a_shaderDir);

	if (a_shaderDir == NULL || src == NULL)
		return -1;

	uint32_t shaderID = glCreateShader((uint32_t)a_type);

	glShaderSource(shaderID, 1, (const_pChar*)&src, 0);
	glCompileShader(shaderID);

	if (!CheckShaderStatus(shaderID)) 
	{
		printf("ERROR: Shader source directory invalid.\n");
		return -1;
	}

	printf("SUCCESS: Shader initialised: %s\n", a_shaderDir);
	return shaderID;
}

const void ShaderHandler::SetUpCameraUniforms(
	const f32mat4& a_camProjMat,
	const f32mat4& a_camViewMat,
	const f32mat4& a_camWorldMat)
{
	//unsigned int uiError;
	for (auto& it : m_programMap)
	{
		glUseProgram(it.second);

		GLint loc = glGetUniformLocation(it.second, "Projection");
		glUniformMatrix4fv(loc, 1, false, &a_camProjMat[0][0]);

		loc = glGetUniformLocation(it.second, "View");
		glUniformMatrix4fv(loc, 1, false, &a_camViewMat[0][0]);

		loc = glGetUniformLocation(it.second, "World");
		glUniformMatrix4fv(loc, 1, false, &a_camWorldMat[0][0]);
	}
}

const void ShaderHandler::SetUpLightingUniforms(
	const f32vec3& a_ambientLight,
	const f32vec3& a_SunPos,
	const float32_t& a_strtLightingHeight,
	const float32_t& a_elapsedTime)
{
	//unsigned int uiError;
	for (auto& it : m_programMap)
	{
		glUseProgram(it.second);

		int32_t loc = glGetUniformLocation(it.second, "AmbientLight");
		glUniform3fv(loc, 1, &a_ambientLight[0]);

		loc = glGetUniformLocation(it.second, "SunPos");
		glUniform3fv(loc, 1, &a_SunPos[0]);
		
		loc = glGetUniformLocation(it.second, "LightingHeight");
		glUniform1f(loc, a_strtLightingHeight);

		loc = glGetUniformLocation(it.second, "time");
		glUniform1f(loc, a_elapsedTime);
	}
}

const void ShaderHandler::ReloadAllPrograms()
{
	for (auto& it : m_programMap)
	{
		const_pChar name = it.first;
		DirectoryData& dirData = m_directoryMap[it.second];
		glDeleteProgram(it.second);

		LoadShaderProgram(name,
			dirData.vertDir,
			dirData.pixeDir,
			dirData.geomDir,
			dirData.tesCDir,
			dirData.tesEDir,
			false);
	}
}

const void ShaderHandler::UnloadAllPrograms()
{
	for (auto& prog : m_programMap)
	{
		glDeleteProgram(prog.second);
	}

	m_programMap.clear();

	glUseProgram(0);
}

// Loops through the map to define if a shader already exists
const bool ShaderHandler::DoesShaderExist(const_pChar& a_fileName)
{
	if (m_programMap.find(a_fileName) == m_programMap.end())
		return false;

	return true;
}

// Checks the success of the link status within the program
const bool ShaderHandler::CheckProgramStatus(const uint32_t &a_prog)
{
	// Retrieve success of linking from program object
	int32_t success = GL_FALSE;
	glGetProgramiv(a_prog, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		// Retrieve the log size
		int32_t infoLogLength = 0;
		glGetProgramiv(a_prog, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		// Print out the log
		glGetProgramInfoLog(a_prog, infoLogLength, 0, infoLog);
		printf("Error: Failed to load program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
		return false;
	}

	// Program is successful!
	return true;
}

// Checks the success of the compile status within the shader
const bool ShaderHandler::CheckShaderStatus(const uint32_t &a_shader)
{
	// Retrieve success of compiling from shader object
	int32_t success = GL_FALSE;
	glGetShaderiv(a_shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		// Retrieve the log size
		int32_t infoLogLength = 0;
		glGetShaderiv(a_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		// Print out the log
		glGetShaderInfoLog(a_shader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
		return false;
	}
	return true;
}

// Readers in the shader from a file source
const_pChar ShaderHandler::ReadShaderCode(const_pChar a_filePath)
{
	if (!a_filePath)
		return NULL;

	// Grab the shader source
	FILE* file = fopen(a_filePath, "rb");
	if (file == NULL)
		return NULL;

	// If it's a bad file, get outta there.
	if (fseek(file, 0, SEEK_END) == -1) 
		return NULL;
	
	uint length = ftell(file);

	if (length == -1) 
		return NULL;
	
	if (fseek(file, 0, SEEK_SET) == -1) 
		return NULL;

	// Populate return char string
	char* content = new char[length + 1];
	memset(content, 0, length + 1);
	
	if (content == NULL) 
		return NULL;
	
	fread(content, sizeof(char), length, file);

	// Check for errors
	if (ferror(file))
	{
		free(content);
		return NULL;
	}

	// Delete pointer data
	fclose(file);
	content[length] = '\0';

	// Successful load!
	return content;
}