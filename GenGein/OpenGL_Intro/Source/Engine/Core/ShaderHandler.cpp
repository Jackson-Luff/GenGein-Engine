
#include <fstream>
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

#include "ShaderHandler.h"

typedef std::map<c_charp, uint> shaderMap;
typedef std::map<uint, ShaderHandler::DirectoryData> directoryMap;
shaderMap ShaderHandler::m_programMap = shaderMap();
directoryMap ShaderHandler::m_directoryMap = directoryMap();

// Deconstructor
ShaderHandler::~ShaderHandler()
{
	// null program
	glUseProgram(0);
}

//Returns shader based on naming
uint& ShaderHandler::GetShader(c_charp a_shaderName)
{
	if (DoesShaderExist(a_shaderName)) return m_programMap[a_shaderName];

	printf("No shader exists for that name!\n");
}

// Create the program with shader file directories
uint ShaderHandler::LoadShaderProgram(c_charp a_shaderName,
	c_charp a_vertexShader, c_charp a_pixelShader,
	c_charp a_geometryShader, c_charp a_tessCntrlShader,
	c_charp a_tessEvalShader )
{
	//if (DoesShaderExist(a_shaderName)) return m_programMap[a_shaderName];

	c_uint vertShader = 
		CreateShader(a_vertexShader, GL_VERTEX_SHADER);
	c_uint fragShader = 
		CreateShader(a_pixelShader, GL_FRAGMENT_SHADER);
	c_uint geoShader = 
		CreateShader(a_geometryShader, GL_GEOMETRY_SHADER);
	c_uint tessCShader = 
		CreateShader(a_tessCntrlShader, GL_TESS_CONTROL_SHADER);
	c_uint tessEShader = 
		CreateShader(a_tessEvalShader, GL_TESS_EVALUATION_SHADER);

	if (vertShader >= 100 || fragShader >= 100)
		return -1;

	uint progID;
	// Apply shaders to program
	progID = glCreateProgram();
	glAttachShader(progID, vertShader);
	glAttachShader(progID, fragShader);
	glAttachShader(progID, geoShader);
	glAttachShader(progID, tessCShader);
	glAttachShader(progID, tessEShader);
	glLinkProgram(progID);
	/*
	glBindAttribLocation(a_program, 0, "vertPosition");
	glBindAttribLocation(a_program, 1, "vertColour");
	glBindAttribLocation(a_program, 2, "vertNormal");
	glBindAttribLocation(a_program, 3, "vertTangent");
	glBindAttribLocation(a_program, 4, "vertBiNormal");
	glBindAttribLocation(a_program, 5, "vertTextCoords");*/

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
uint ShaderHandler::CreateShader(c_charp a_shaderDir, c_uint a_type)
{
	c_charp src = ReadShaderCode(a_shaderDir);

	if (a_shaderDir == NULL || src == NULL)
		return -1;

	uint shaderID = glCreateShader(a_type);

	glShaderSource(shaderID, 1, (c_charp*)&src, 0);
	glCompileShader(shaderID);

	if (!CheckShaderStatus(shaderID)) {
		printf("ERROR: Shader source directory invalid.\n");
		return -1;
	}

	printf("SUCCESS: Shader initialised: %s\n", a_shaderDir);
	return shaderID;
}

const void ShaderHandler::SetUpUniformData(const glm::mat4& a_camProjMat,
	const glm::mat4& a_camViewMat,
	const glm::mat4& a_camWorldMat,
	const glm::vec4& a_camEyePos,
	const float& a_elapsedTime)
{
	//unsigned int uiError;
	for (auto it = m_programMap.begin(); it != m_programMap.end(); ++it)
	{
		glUseProgram(it->second);

		GLint loc = glGetUniformLocation(it->second, "Projection");
		glUniformMatrix4fv(loc, 1, false, &a_camProjMat[0][0]);

		loc = glGetUniformLocation(it->second, "View");
		glUniformMatrix4fv(loc, 1, false, &a_camViewMat[0][0]);

		loc = glGetUniformLocation(it->second, "World");
		glUniformMatrix4fv(loc, 1, false, &a_camWorldMat[0][0]);

		loc = glGetUniformLocation(it->second, "EyePos");
		glUniform4fv(loc, 1, &a_camEyePos[0]);

		loc = glGetUniformLocation(it->second, "time");
		glUniform1f(loc, a_elapsedTime);
	}
}

const void ShaderHandler::ReloadAllPrograms()
{
	for (auto it : m_programMap)
	{
		if (it.first == "SkyBox") continue;

		glDeleteProgram(it.second);
		DirectoryData dirData = m_directoryMap[it.second];
		
		LoadShaderProgram(it.first,
			dirData.vertDir,
			dirData.pixeDir,
			dirData.geomDir,
			dirData.tesCDir,
			dirData.tesEDir);
	}
}

// Loops through the map to define if a shader already exists
const bool ShaderHandler::DoesShaderExist(c_charp& a_fileName)
{
	if (m_programMap.find(a_fileName) == m_programMap.end())
		return false;

	return true;
}

// Checks the success of the link status within the program
const bool ShaderHandler::CheckProgramStatus(c_uint &a_prog)
{
	// Retrieve success of linking from program object
	int success = GL_FALSE;
	glGetProgramiv(a_prog, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		// Retrieve the log size
		int infoLogLength = 0;
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
const bool ShaderHandler::CheckShaderStatus(c_uint &a_shader)
{
	// Retrieve success of compiling from shader object
	int success = GL_FALSE;
	glGetShaderiv(a_shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		// Retrieve the log size
		int infoLogLength = 0;
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
c_charp ShaderHandler::ReadShaderCode(c_charp a_filePath)
{
	if (!a_filePath)
		return NULL;

	// Grab the shader source
	FILE* file = fopen(a_filePath, "rb");
	if (file == NULL)
		return NULL;

	// If it's a bad file, get outta there.
	if (fseek(file, 0, SEEK_END) == -1) return NULL;
	uint length = ftell(file);
	if (length == -1) return NULL;
	if (fseek(file, 0, SEEK_SET) == -1) return NULL;

	// Populate return char string
	char* content = new char[length + 1];
	memset(content, 0, length + 1);
	if (content == NULL) return NULL;
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