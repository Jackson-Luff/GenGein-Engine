#include <string>
#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"
#include "ObjectLoader.h"
#include "BaseObject.h"

#include "ObjMesh.h"

//  Default Constructor
ObjMesh::ObjMesh()
	: BaseObject()
{}

//  Constructor
ObjMesh::ObjMesh(vec3 a_position) 
	: BaseObject(a_position)
{}

// Constructor with folder and file directory
void ObjMesh::LoadObject( c_charp a_folderDir, c_charp a_fileName )
{
	/*
	c_charp folderDir;
	c_charp fileName;
	bool fileNameBuff = true;

	for (uint i = strlen(a_fileDir)-1; i <= 0; --i)
	{
		if (fileNameBuff)
			fileName += a_fileDir[i];
		else
			folderDir += a_fileDir[i];
	}*/

	m_obj = new ObjectLoader( a_folderDir, a_fileName);

	CalcTangentNBiNormals();

	ApplyDataToVertNIndexBuffers(m_obj);

	m_obj->CleanUp();
}

// Same Constructor but accepts custom texture name
void ObjMesh::LoadObject( c_charp a_folderDir, c_charp a_fileDir, c_charp a_textureName)
{
	/*
	c_charp folderDir;
	c_charp fileName;
	bool fileNameBuff = true;

	for (uint i = strlen(a_fileDir) - 1; i <= 0; --i)
	{
		if (fileNameBuff)
			fileName += a_fileDir[i];
		else
			folderDir += a_fileDir[i];
	}*/

	m_obj = new ObjectLoader( a_folderDir, a_fileDir, a_textureName);
	CalcTangentNBiNormals();

	ApplyDataToVertNIndexBuffers(m_obj);

	m_obj->CleanUp();
}

// Deconstructor
ObjMesh::~ObjMesh()
{
	delete m_obj;
}

void ObjMesh::ApplyDataToVertNIndexBuffers(ObjectLoader* a_obj )
{
	ShaderHandler::LoadShaderProgram("ParticleDrawShader",
		"Data/Shaders/ObjShader.vert",
		"Data/Shaders/ObjShader.frag");
	m_programID = ShaderHandler::GetShader("ParticleDrawShader");

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray( m_VAO );
	
	// Generate and bind buffers to the vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (m_obj->GetMesh()->vertices.size() * sizeof(ObjVertex)), &m_obj->GetMesh()->vertices[0], GL_STATIC_DRAW);
	
	// (0) - (vec4)Positon | (1) - (vec4)Colour   | (2) - (vec4)Normal
	// (3) - (vec4)Tangent | (4) - (vec4)BiNormal | (5) - (vec4)TextCoords
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), ((char*)0) + (sizeof(glm::vec4) * 0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), ((char*)0) + (sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), ((char*)0) + (sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), ((char*)0) + (sizeof(glm::vec4) * 3));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), ((char*)0) + (sizeof(glm::vec4) * 4));
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(ObjVertex), ((char*)0) + (sizeof(glm::vec4) * 5));

	m_indexCount = m_obj->GetMesh()->indices.size();

	// Generate and bind the to the index array object
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(GLuint), &m_obj->GetMesh()->indices[0], GL_STATIC_DRAW);
	
	// Null the binded vert array
	glBindVertexArray(0);
}

void ObjMesh::CalcTangentNBiNormals()
{
	std::vector<ObjVertex>&verts = m_obj->GetMesh()->vertices;
	std::vector<  uint   >&index = m_obj->GetMesh()->indices;

	for(uint i = 0; i < m_obj->GetMesh()->indices.size()-3; i+=3)
	{
		// Create triangulation of vert positions 
		glm::vec4 v0 = verts[index[i + 0]].position;
		glm::vec4 v1 = verts[index[i + 1]].position;
		glm::vec4 v2 = verts[index[i + 2]].position;

		// Create triangulation of vert uv's
		glm::vec2 uv0 = verts[index[i + 0]].uv;
		glm::vec2 uv1 = verts[index[i + 1]].uv;
		glm::vec2 uv2 = verts[index[i + 2]].uv;

		// Edges of the triangle : position delta
		glm::vec4 deltaPos1 = v1 - v0;
		glm::vec4 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec4 t = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec4 bi = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		m_obj->GetMesh()->vertices[index[i+0]].binormal = bi;
		m_obj->GetMesh()->vertices[index[i+1]].binormal = bi;
		m_obj->GetMesh()->vertices[index[i+2]].binormal = bi;
	
		m_obj->GetMesh()->vertices[index[i+0]].tangent = t;
		m_obj->GetMesh()->vertices[index[i+1]].tangent = t;
		m_obj->GetMesh()->vertices[index[i+2]].tangent = t;
	}
}

// Draw mesh and send textures to shader
void ObjMesh::Render()
{
	glUseProgram(m_programID);

	//Rebind VAO
	glBindVertexArray(m_VAO);

	//Drawing the object
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}
