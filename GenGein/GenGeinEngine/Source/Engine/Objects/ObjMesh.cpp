#include <fstream>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"
#include "Engine\Renderer\TextureHandler.h"
#include "ObjMesh.h"
#include "BaseObject.h"

#include "OBJMesh.h"

//  Default Constructor
ObjMesh::ObjMesh() :
m_currentMaterial(0),
m_hasUV(false),
m_hasNormals(false),
m_isQuads(false),
m_indexChecked(false),
m_folderDirectory(""),
m_materials(0),
m_vertexMap()
{}

// Constructor with folder and file directory
void ObjMesh::LoadObject( c_pChar a_folderDir, c_pChar a_fileName )
{
	// Initialise bool's
	m_hasUV = false;
	m_hasNormals = false;
	m_indexChecked = false;
	m_isQuads = false;

	ShaderHandler::LoadShaderProgram("ObjShader",
		"Data/Shaders/Geometry/ObjShader.vert",
		"Data/Shaders/Geometry/ObjShader.frag");
	m_programID = &ShaderHandler::GetShader("ObjShader");

	// Apply the material based upon the input directory
	m_folderDirectory = a_folderDir;
	loadMaterials(m_folderDirectory + "/" + a_fileName + ".mtl");
	loadObjects(m_folderDirectory + "/" + a_fileName + ".obj");

	CalcTangentNBiNormals();
	ApplyDataToVertNIndexBuffers();

	m_localMatUniLoc = glGetUniformLocation(*m_programID, "LocalMatrix");

	CleanUp();
}

// Same Constructor but accepts custom texture name
void ObjMesh::LoadObject( c_pChar a_folderDir, c_pChar a_fileName, c_pChar a_textureName)
{
	/*
	c_pChar folderDir;
	c_pChar fileName;
	bool fileNameBuff = true;

	for (uint i = strlen(a_fileDir) - 1; i <= 0; --i)
	{
		if (fileNameBuff)
			fileName += a_fileDir[i];
		else
			folderDir += a_fileDir[i];
	}*/

	// Initialise bool's
	m_hasUV = false;
	m_hasNormals = false;
	m_indexChecked = false;
	m_isQuads = false;

	ShaderHandler::LoadShaderProgram("ObjShader",
		"Data/Shaders/Geometry/ObjShader.vert",
		"Data/Shaders/Geometry/ObjShader.frag");
	m_programID = &ShaderHandler::GetShader("ObjShader");

	// Apply the material based upon the input directory
	m_folderDirectory = a_folderDir;
	loadMaterials(m_folderDirectory + "/" + a_fileName + ".mtl");
	loadObjects(m_folderDirectory + "/" + a_fileName + ".obj");

	CalcTangentNBiNormals();

	ApplyDataToVertNIndexBuffers();

	CleanUp();

	TextureHandler::LoadTexture("ObjShader", "diffuseMap", a_textureName);
}

// Deconstructor
ObjMesh::~ObjMesh()
{}

void ObjMesh::ApplyDataToVertNIndexBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray( m_VAO );
	
	// Generate and bind buffers to the vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (m_OBJ_verts.size() * sizeof(OBJVertex)), &m_OBJ_verts[0], GL_STATIC_DRAW);
	
	// (0) - (vec4)Positon | (1) - (vec4)Colour   | (2) - (vec4)Normal
	// (3) - (vec4)Tangent | (4) - (vec4)BiNormal | (5) - (vec4)TextCoords
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + (sizeof(f32vec4) * 0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + (sizeof(f32vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + (sizeof(f32vec4) * 2));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + (sizeof(f32vec4) * 3));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + (sizeof(f32vec4) * 4));
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + (sizeof(f32vec4) * 5));

	m_indexCount = m_indices.size();

	// Generate and bind the to the index array object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);
	
	// Null the binded vert array
	glBindVertexArray(0);
}

void ObjMesh::CalcTangentNBiNormals()
{
	std::vector<OBJVertex>& verts = m_OBJ_verts;
	std::vector<uint32_t>& index = m_indices;

	for (uint32_t i = 0; i < index.size() - 3; i += 3)
	{
		// Create triangulation of vert positions 
		f32vec4 v0 = verts[index[i + 0]].position;
		f32vec4 v1 = verts[index[i + 1]].position;
		f32vec4 v2 = verts[index[i + 2]].position;

		// Create triangulation of vert uv's
		f32vec2 uv0 = verts[index[i + 0]].uv;
		f32vec2 uv1 = verts[index[i + 1]].uv;
		f32vec2 uv2 = verts[index[i + 2]].uv;

		// Edges of the triangle : position delta
		f32vec4 deltaPos1 = v1 - v0;
		f32vec4 deltaPos2 = v2 - v0;

		// UV delta
		f32vec2 deltaUV1 = uv1 - uv0;
		f32vec2 deltaUV2 = uv2 - uv0;

		float32_t r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		f32vec4 t = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		f32vec4 bi = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		verts[index[i+0]].binormal = bi;
		verts[index[i+1]].binormal = bi;
		verts[index[i+2]].binormal = bi;
		
		verts[index[i+0]].tangent = t;
		verts[index[i+1]].tangent = t;
		verts[index[i+2]].tangent = t;
	}
}

// Draw mesh and send Rendererto shader
void ObjMesh::Render()
{
	glUseProgram(*m_programID);

	glUniformMatrix4fv(m_localMatUniLoc, 1, GL_FALSE, &m_localTrans[0][0]);

	//Rebind VAO
	glBindVertexArray(m_VAO);

	//Drawing the object
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void ObjMesh::CleanUp()
{
	// TODO: CLEAN PRE-PROCESSESS BULLSHIT HERE
}

std::string ObjMesh::parseString(c_string a_src, c_string code)
{
	// Source the inputted string to return a 'wanted' string
	char buffer[64];
	std::string scanStr = code + " %s";
	sscanf_s(a_src.c_str(), scanStr.c_str(), &buffer, _countof(buffer));
	return std::string(buffer);
}

float32_t ObjMesh::parseFloat(c_string a_src, c_string a_code)
{
	// Source the inputted string to return a 'wanted' float
	float32_t trait;
	std::string scanStr = a_code + " %f";
	sscanf_s(a_src.c_str(), scanStr.c_str(), &trait);
	return trait;
}

f32vec2 ObjMesh::parseVec2(c_string a_src, c_string a_code)
{
	// Source the inputted string to return a 'wanted' vector2
	float32_t x, y;
	std::string scanStr = a_code + " %f %f";
	sscanf_s(a_src.c_str(), scanStr.c_str(), &x, &y);
	return f32vec2(x, y);
}

f32vec3 ObjMesh::parseVec3(c_string a_src, c_string a_code)
{
	// Source the inputted string to return a 'wanted' vector3
	float32_t x, y, z;
	std::string scanStr = a_code + " %f %f %f";
	sscanf_s(a_src.c_str(), scanStr.c_str(), &x, &y, &z);
	return f32vec3(x, y, z);
}

void ObjMesh::checkIndices(c_string a_str)
{
	// Used once to initialise face type
	if (!m_indexChecked)
	{
		int32_t bestSample = 0;
		int32_t sampleV1 = -1, sampleU1 = -1, sampleN1 = -1;
		int32_t tmp = -1, sampleV4 = -1;

		// If the sampled data is found, form'X' will return 1
		int32_t form1 = sscanf(a_str.c_str(), "f %d", &sampleV1);
		int32_t form2 = sscanf(a_str.c_str(), "f %d/%d ", &sampleV1, &sampleU1);
		int32_t form3 = sscanf(a_str.c_str(), "f %d//%d", &sampleV1, &sampleN1);
		int32_t form4 = sscanf(a_str.c_str(), "f %d/%d/%d", &sampleV1, &sampleU1, &sampleN1);

		// Check if form1 was the sample
		// and set bools accordingly
		if (form1 > bestSample)
		{
			bestSample = form1;
			m_hasUV = false;
			m_hasNormals = false;
		}

		// Check if form2 was the sample
		// and set bools accordingly
		if (form2 > bestSample)
		{
			bestSample = form2;
			m_hasUV = true;
			m_hasNormals = false;
		}

		// Check if form3 was the sample
		// and set bools accordingly
		if (form3 > bestSample)
		{
			bestSample = form3;
			m_hasUV = false;
			m_hasNormals = true;
		}

		// Check if form4 was the sample
		// and set bools accordingly
		if (form4 > bestSample)
		{
			bestSample = form4;
			m_hasUV = true;
			m_hasNormals = true;
		}

		// if a sample exists, success.. Else cry
		m_indexChecked = true;
	}
}

void ObjMesh::addIndices(c_string a_str)
{
	i32vec4 v(-1), u(-1), n(-1);

	// Check indices for sampling the face data 
	checkIndices(a_str);

	if (!m_hasUV && !m_hasNormals)
	{
		//if ONLY vertice info, populate the data
		sscanf_s(a_str.c_str(), "f %d %d %d %d",
			&v.x,
			&v.y,
			&v.z,
			&v.w);
	}
	else if (m_hasUV && !m_hasNormals)
	{
		//if ONLY vertice info and uv info, populate the data
		sscanf_s(a_str.c_str(), "f %d/%d %d/%d %d/%d %d/%d",
			&v.x, &u.x,
			&v.y, &u.y,
			&v.z, &u.z,
			&v.w, &u.w);
	}
	else if (!m_hasUV && m_hasNormals)
	{
		//if ONLY vertice info and normal info, populate the data
		sscanf_s(a_str.c_str(), "f %d//%d %d//%d %d//%d %d//%d",
			&v.x, &n.x,
			&v.y, &n.y,
			&v.z, &n.z,
			&v.w, &n.w);
	}
	else if (m_hasUV && m_hasNormals)
	{
		//if the face has all attributes, populate the data
		sscanf_s(a_str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
			&v.x, &u.x, &n.x,
			&v.y, &u.y, &n.y,
			&v.z, &u.z, &n.z,
			&v.w, &u.w, &n.w);
	}

	// Subtract one to convert indices -
	// info into index capatible data
	v--; u--; n--;

	//Apply indexed triangle(s)
	if (v.x >= 0)
	{
		std::string key;

		//First Triangle
		// Grab the [0] key
		key = std::to_string(v.x) + "/" + std::to_string(u.x) + "/" + std::to_string(n.x);
		ApplyIndexInfo(key);
		// Grab the [1] key
		key = std::to_string(v.y) + "/" + std::to_string(u.y) + "/" + std::to_string(n.y);
		ApplyIndexInfo(key);
		// Grab the [2] key
		key = std::to_string(v.z) + "/" + std::to_string(u.z) + "/" + std::to_string(n.z);
		ApplyIndexInfo(key);

		if (v.w >= 0)
		{
			m_isQuads = true;

			//Second Triangle (For Quads ONLY)
			// Grab the [2] key
			key = std::to_string(v.z) + "/" + std::to_string(u.z) + "/" + std::to_string(n.z);
			ApplyIndexInfo(key);
			// Grab the [3] key
			key = std::to_string(v.w) + "/" + std::to_string(u.w) + "/" + std::to_string(n.w);
			ApplyIndexInfo(key);
			// Grab the [0] key
			key = std::to_string(v.x) + "/" + std::to_string(u.x) + "/" + std::to_string(n.x);
			ApplyIndexInfo(key);
		}
	}
}

void ObjMesh::ApplyIndexInfo(c_string a_key)
{
	i32vec3 vI;

	// vI.x : position, vI.y : uv, vI.z : normal.
	sscanf(a_key.c_str(), "%d/%d/%d", &vI.x, &vI.y, &vI.z);

	for (int32_t i = 0; i < 3; i++)
		if (vI[i] < 0) vI[i] = 0;

	// Checking to see if that vertex already exists 
	if (m_vertexMap.find(a_key) != m_vertexMap.end())
	{
		// Vertex exists!
		m_indices.push_back(m_vertexMap[a_key]);
		return;
	}
	else
	{
		// It's a new vertex!
		OBJVertex newVert;
		newVert.position = m_object.position[vI.x];
		newVert.colour = f32vec4(1);

		if (m_hasUV)
			newVert.uv = m_object.texCoords[vI.y];

		if (m_hasNormals)
			newVert.normal = m_object.normals[vI.z];


		// Push back the new vertex 
		m_OBJ_verts.push_back(newVert);

		// Push back the index for the new vertex
		uint32_t uiIndex = m_OBJ_verts.size() - 1;
		m_indices.push_back(uiIndex);

		// Pair this key to that unique index
		m_vertexMap.insert(std::pair<const std::string, uint32_t>(a_key, uiIndex));
	}
}

void ObjMesh::loadObjects(c_string a_objPath)
{
	// Create a file container based on directory
	std::ifstream file(a_objPath);

	std::string buffer;
	m_object = Temp_OBJ_Data();

	if (m_materials.size() != 0)
		m_currentMaterial = m_materials[0];

	if (file.bad())
	{
		printf("Unable to find path: %s\n", a_objPath.c_str());
		return;
	}

	// Read through every line within the file
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, buffer);

			//Grabs data based on the stored .obj flags
			if (buffer.find("v ") == 0)
				m_object.position.push_back(f32vec4(parseVec3(buffer, "v"), 1.0f));
			else if (buffer.find("vn") == 0)
				m_object.normals.push_back(f32vec4(parseVec3(buffer, "vn"), 1.0f));
			else if (buffer.find("vt") == 0)
			{
				f32vec2 vt = parseVec2(buffer, "vt");
				vt.y *= -1;
				m_object.texCoords.push_back(vt);
			}
			else if (buffer.find("usemtl ") == 0)
			{
				std::string name = parseString(buffer, "usemtl ");
				for (uint32_t i = 0; i < m_materials.size(); i++)
				{
					if (name == m_materials[i]->name)
					{
						if (i >= 1)
							m_materials[i - 1] = m_currentMaterial;

						m_currentMaterial = m_materials[i];
					}
				}
			}
			else if (buffer.find("f ") == 0)
				addIndices(buffer);
			else if (buffer.find("# ") == 0)
				printf("%s \n", buffer.c_str());
		}
		printf("Object Load: successful...\n");
		m_currentMaterial = NULL;
		delete m_currentMaterial;
	}
}

void ObjMesh::loadMaterials(c_string a_matPath)
{
	// Creates flie based on directory
	std::ifstream file(a_matPath);
	std::string buffer;
	m_currentMaterial = NULL;

	if (file.bad())
	{
		printf("Unable to find path: %s\n", a_matPath.c_str());
		return;
	}

	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, buffer);

			if (buffer.find("newmtl ") == 0)
			{
				if (m_currentMaterial != NULL)
					m_materials.push_back(m_currentMaterial);

				m_currentMaterial = new Material();
				m_currentMaterial->name = parseString(buffer, "newmtl ");
			}
			else if (buffer.find("Ka ") == 0)
				m_currentMaterial->Ka = parseVec3(buffer, "Ka ");
			else if (buffer.find("Kd ") == 0)
				m_currentMaterial->Kd = parseVec3(buffer, "Kd ");
			else if (buffer.find("Ks ") == 0)
				m_currentMaterial->Ks = parseVec3(buffer, "Ks ");
			else if (buffer.find("d ") == 0)
				m_currentMaterial->d = parseFloat(buffer, "d ");
			else if (buffer.find("Ns ") == 0)
				m_currentMaterial->Ns = parseFloat(buffer, "Ns ");
			else if (buffer.find("illum ") == 0)
				m_currentMaterial->illum = parseFloat(buffer, "illum ");
			else if (buffer.find("map_Ka") == 0)
				TextureHandler::LoadTexture("ObjShader", "ambientMap", m_folderDirectory + "/" + parseString(buffer, "map_Ka "));
			else if (buffer.find("map_Kd") == 0)
				TextureHandler::LoadTexture("ObjShader", "diffuseMap", m_folderDirectory + "/" + parseString(buffer, "map_Kd "));
			else if (buffer.find("map_Ks") == 0)
				TextureHandler::LoadTexture("ObjShader", "specularMap", m_folderDirectory + "/" + parseString(buffer, "map_Ks "));
			else if (buffer.find("map_Ns") == 0)
				TextureHandler::LoadTexture("ObjShader", "normalMap", m_folderDirectory + "/" + parseString(buffer, "map_Ns "));
		}

		if (m_currentMaterial != NULL)
			m_materials.push_back(m_currentMaterial);
	}
}