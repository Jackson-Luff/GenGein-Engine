
#include <fstream>
#include <vector>
#include <glm\glm.hpp>

#include "Engine\Textures\TextureHandler.h"
#include "Engine\Core\ShaderHandler.h"

#include "ObjectLoader.h"

ObjectLoader::ObjectLoader() :
	m_currentMaterial(0),
	m_object(0),
	m_hasUV(false),
	m_hasNormals(false),
	m_isQuads(false),
	m_indexChecked(false),
	m_folderDirectory(""),
	m_materials(0),
	m_vertexMap()
{}

ObjectLoader::ObjectLoader( c_string a_folderName, c_string a_fileName)
{
	// Initialise bool's
	m_hasUV = false;
	m_hasNormals = false;
	m_indexChecked = false;
	m_isQuads = false;

	// Apply the material based upon the input directory
	m_folderDirectory = a_folderName;
	loadMaterials( m_folderDirectory + "/" + a_fileName + ".mtl");
	loadObjects( m_folderDirectory + "/" + a_fileName+ ".obj");

	ShaderHandler::LoadShaderProgram("ObjShader",
		"Data/Shaders/ObjShader.vert",
		"Data/Shaders/ObjShader.frag");
}
							
ObjectLoader::ObjectLoader( c_string a_folderName, c_string a_fileName, c_string a_textureName ) 
{
	// Initialise bool's
	m_hasUV = false;
	m_hasNormals = false;
	m_indexChecked = false;
	m_isQuads = false;

	// Initiate folder directory
	m_folderDirectory = "Data/Objects/" + a_folderName;

	ShaderHandler::LoadShaderProgram("ObjShader",
		"Data/Shaders/ObjShader.vert",
		"Data/Shaders/ObjShader.frag");

	// Use a single material based on texture directory input
	Material* defaultMat = new Material();
	defaultMat->name = "texture";
	TextureHandler::LoadTexture(m_shaderID, "diffuseMap", m_folderDirectory + a_fileName);

	m_materials.push_back(defaultMat);

	// Load up the .obj based on directory input
	loadObjects( m_folderDirectory + "/" + a_fileName + ".obj");
}

ObjectLoader::~ObjectLoader() 
{
	m_materials.clear();
}

void ObjectLoader::CleanUp()
{
	delete m_object;
}

std::string ObjectLoader::parseString( c_string a_src, c_string code )
{
	// Source the inputted string to return a 'wanted' string
    char buffer[64];
    std::string scanStr = code + " %s";
    sscanf_s(a_src.c_str(), scanStr.c_str(), &buffer, _countof(buffer));
    return std::string(buffer);
}

float ObjectLoader::parseFloat( c_string a_src, c_string a_code )
{
	// Source the inputted string to return a 'wanted' float
    float trait;
    std::string scanStr = a_code + " %f";
    sscanf_s(a_src.c_str(), scanStr.c_str(), &trait);
    return trait;
}

glm::vec2 ObjectLoader::parseVec2( c_string a_src, c_string a_code ) 
{
	// Source the inputted string to return a 'wanted' vector2
	float x, y;
	std::string scanStr = a_code + " %f %f";
	sscanf_s(a_src.c_str(), scanStr.c_str(), &x, &y);
	return glm::vec2(x, y);
}

glm::vec3 ObjectLoader::parseVec3( c_string a_src, c_string a_code ) 
{
	// Source the inputted string to return a 'wanted' vector3
	float x, y, z;
	std::string scanStr = a_code + " %f %f %f";
	sscanf_s(a_src.c_str(), scanStr.c_str(), &x, &y, &z);
	return glm::vec3(x, y, z);
}

void ObjectLoader::checkIndices( c_string a_str )
{
	// Used once to initialise face type
	if(!m_indexChecked) 
	{
		int bestSample = 0;
		int sampleV1 = -1, sampleU1 = -1, sampleN1 = -1;
		int tmp = -1, sampleV4 = -1;
		
		// If the sampled data is found, form'X' will return 1
		int form1 = sscanf( a_str.c_str(), "f %d", &sampleV1 );
		int form2 = sscanf( a_str.c_str(), "f %d/%d ", &sampleV1, &sampleU1 );
		int form3 = sscanf( a_str.c_str(), "f %d//%d", &sampleV1, &sampleN1 );
		int form4 = sscanf( a_str.c_str(), "f %d/%d/%d", &sampleV1, &sampleU1, &sampleN1 );

		// Check if form1 was the sample
		// and set bools accordingly
		if(form1 > bestSample) 
		{
			bestSample = form1;
			m_hasUV = false;
			m_hasNormals = false;
		}

		// Check if form2 was the sample
		// and set bools accordingly
		if(form2 > bestSample) 
		{
			bestSample = form2;
			m_hasUV = true;
			m_hasNormals = false;
		}

		// Check if form3 was the sample
		// and set bools accordingly
		if(form3 > bestSample) 
		{
			bestSample = form3;
			m_hasUV = false;
			m_hasNormals = true;
		}

		// Check if form4 was the sample
		// and set bools accordingly
		if(form4 > bestSample) 
		{
			bestSample = form4;
			m_hasUV = true;
			m_hasNormals = true;
		}

		// if a sample exists, success.. Else cry
		m_indexChecked = true;
	}
}

void ObjectLoader::addIndices( c_string a_str )
{
	glm::ivec4 v(-1), u(-1), n(-1);
	
	// Check indices for sampling the face data 
    checkIndices(a_str);

    if (!m_hasUV && !m_hasNormals) 
	{
		//if ONLY vertice info, populate the data
        sscanf_s(a_str.c_str(),"f %d %d %d %d",
            &v.x,
            &v.y,
            &v.z,
            &v.w);
    }
    else if (m_hasUV && !m_hasNormals)
	{
		//if ONLY vertice info and uv info, populate the data
        sscanf_s(a_str.c_str(),"f %d/%d %d/%d %d/%d %d/%d",
            &v.x, &u.x,
            &v.y, &u.y,
            &v.z, &u.z,
            &v.w, &u.w);
    }
    else if (!m_hasUV && m_hasNormals) 
	{
		//if ONLY vertice info and normal info, populate the data
        sscanf_s(a_str.c_str(),"f %d//%d %d//%d %d//%d %d//%d",
            &v.x, &n.x,
            &v.y, &n.y,
            &v.z, &n.z,
            &v.w, &n.w);
    }
    else if (m_hasUV && m_hasNormals) 
	{
		//if the face has all attributes, populate the data
        sscanf_s(a_str.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
            &v.x, &u.x, &n.x,
            &v.y, &u.y, &n.y,
            &v.z, &u.z, &n.z,
            &v.w, &u.w, &n.w);
    }

	// Subtract one to convert indices -
	// info into index capatible data
	v--; u--; n--;

	//Apply indexed triangle(s)
	if(v.z >= 0)
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

		if(v.w >= 0)
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

void ObjectLoader::ApplyIndexInfo( c_string a_key )
{
	glm::ivec3 vI;

	// vI.x : position, vI.y : uv, vI.z : normal.
	sscanf(a_key.c_str(), "%d/%d/%d", &vI.x, &vI.y, &vI.z);
	
	for(int i = 0; i < 3; i++)
		if(vI[i] < 0) vI[i] = 0;

	// Checking to see if that vertex already exists 
	if(m_vertexMap.find(a_key) != m_vertexMap.end())
	{
		// Vertex exists!
		m_object->indices.push_back( m_vertexMap[a_key] );
		return;
	}
	else
	{
		// It's a new vertex!
		ObjVertex newVert;
		newVert.position = m_object->position[vI.x];
		newVert.colour = glm::vec4(1);

		if (m_hasUV)
			newVert.uv		 = m_object->texCoords[vI.y];

		if (m_hasNormals)
			newVert.normal	 = m_object->normals[vI.z];
		

		// Push back the new vertex 
		m_object->vertices.push_back(newVert);

		// Push back the index for the new vertex
		unsigned int uiIndex = m_object->vertices.size() - 1;
		m_object->indices.push_back( uiIndex );

		// Pair this key to that unique index
		m_vertexMap.insert( std::pair<std::string, unsigned int>(a_key, uiIndex) );
	}
}

void ObjectLoader::loadObjects( c_string a_objPath )
{
	// Create a file container based on directory
	std::ifstream file(a_objPath);

	std::string buffer;
	m_object = new ObjectData();

	if(m_materials.size() != 0)
		m_currentMaterial = m_materials[0];

	if(file.bad())
	{
		printf("Unable to find path: %s\n", a_objPath.c_str());
		return;
	}

	// Read through every line within the file
	if(file.is_open()) 
	{
		while(!file.eof()) 
		{
			getline(file, buffer);

			//Grabs data based on the stored .obj flags
			if(buffer.find("v ") == 0)
				m_object->position.push_back(glm::vec4(parseVec3(buffer, "v"), 1.0f));
			else if(buffer.find("vn") == 0)
				m_object->normals.push_back( glm::vec4(parseVec3(buffer, "vn"), 1.0f));
			else if(buffer.find("vt") == 0) 
			{
				glm::vec2 vt = parseVec2(buffer, "vt");
				vt.y *= -1;
				m_object->texCoords.push_back(vt);
			}
			else if(buffer.find("usemtl ") == 0)
			{
				std::string name = parseString(buffer, "usemtl ");
				for(unsigned int i = 0; i < m_materials.size(); i++)
				{
					if( name == m_materials[i]->name)
					{
						if( i >= 1 ) 
							m_materials[i-1] = m_currentMaterial;

						m_currentMaterial = m_materials[i];
					}
				}
			}
			else if(buffer.find("f ") == 0 )
				addIndices(buffer);
			else if(buffer.find("# ") == 0 )
				printf("%s \n", buffer.c_str());
		}
		printf("Object Load: successful...\n");
		m_currentMaterial = NULL;
		delete m_currentMaterial;
	}
}

void ObjectLoader::loadMaterials( c_string a_matPath ) 
{
	// Creates flie based on directory
	std::ifstream file(a_matPath);
	std::string buffer;
	m_currentMaterial = NULL;

	if(file.bad())
	{
		printf("Unable to find path: %s\n", a_matPath.c_str());
		return;
	}

	if(file.is_open())
	{
		while(!file.eof())
		{
			getline(file, buffer);

			if(buffer.find("newmtl ") == 0)
			{
				if(m_currentMaterial != NULL)
					m_materials.push_back(m_currentMaterial);

				m_currentMaterial = new Material();
				m_currentMaterial->name = parseString(buffer, "newmtl ");
			}
			else if(buffer.find("Ka ") == 0)
				m_currentMaterial->Ka = parseVec3(buffer, "Ka ");
			else if(buffer.find("Kd ") == 0)
				m_currentMaterial->Kd = parseVec3(buffer, "Kd ");
			else if(buffer.find("Ks ") == 0)
				m_currentMaterial->Ks= parseVec3(buffer, "Ks ");
			else if(buffer.find("d ") == 0)
				m_currentMaterial->d = parseFloat(buffer, "d ");
			else if(buffer.find("Ns ") == 0)
				m_currentMaterial->Ns = parseFloat(buffer, "Ns ");
			else if (buffer.find("illum ") == 0)
				m_currentMaterial->illum = parseFloat(buffer, "illum ");
			else if (buffer.find("map_Ka") == 0)
				TextureHandler::LoadTexture(m_shaderID, "ambientMap", m_folderDirectory + parseString(buffer, "map_Ka "));
			else if (buffer.find("map_Kd") == 0)
				TextureHandler::LoadTexture(m_shaderID, "diffuseMap", m_folderDirectory + parseString(buffer, "map_Ka "));
			else if (buffer.find("map_Ks") == 0)
				TextureHandler::LoadTexture(m_shaderID, "specularMap", m_folderDirectory + parseString(buffer, "map_Ka "));
			else if (buffer.find("map_Ns") == 0)
				TextureHandler::LoadTexture(m_shaderID, "normalMap", m_folderDirectory + parseString(buffer, "map_Ka "));
		}

		if(m_currentMaterial != NULL)
			m_materials.push_back(m_currentMaterial);
	}
}