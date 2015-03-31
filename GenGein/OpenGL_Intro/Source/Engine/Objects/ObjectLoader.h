#pragma once
#include <map>
#include <vector>
#include <string>
#include <glm\glm.hpp>

#include "Engine\Textures\TextureHandler.h"

typedef const std::string c_string;

////
// Author: Jackson Luff
// Name: ObjectLoader
// Type: class (standard)
// Parent: None
// Description:
// * ObjectLoader is a some-what advanced .obj 
// * file formate loader. It is compatible with
// * both tri's and quad's and is probably more
// * complex than it needs to be.
////

// Vertex holds the layout info for the VBO
struct ObjVertex
{
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec4 binormal;
	glm::vec2 uv;
};

class ObjectLoader
{
private:

	// Holds all attributes of the mesh
	struct ObjectData
	{
		std::vector<glm::vec4> position, normals, colours;
		std::vector<glm::vec2> texCoords;
		std::vector<ObjVertex> vertices;
		std::vector<unsigned int> indices;
	};

	// Holds most attributes of a material
	struct Material
	{
		std::string name;
		glm::vec3 Ka, Kd, Ks;
		float d, Ns, illum;
	};

	// returns float stored after string
	float parseFloat(c_string a_src, c_string a_code);
	// returns vec2 stored after string
	glm::vec2 parseVec2(c_string a_src, c_string a_code);
	// returns vec3 stored after string
	glm::vec3 parseVec3(c_string a_src, c_string a_code);
	// returns string stored after string
	std::string parseString(c_string a_src, c_string a_code);

	// Add indices to the collection
	void addIndices(c_string a_str);
	// Samples type of faces
	void checkIndices(c_string a_str);
	// Search through map for vertex via index key
	void ApplyIndexInfo(c_string a_key);

	// Loads and explores the .obj file format
	void loadObjects(c_string a_objPath);
	// Loads and explores the .mtl file format
	void loadMaterials(c_string a_matPath);

	// Points to current material used
	Material* m_currentMaterial;
	// Points to completed mesh
	ObjectData* m_object;
	// Determines as to whether the texture has these traits
	bool m_hasUV, m_hasNormals, m_isQuads, m_indexChecked;
	// Points to file directory
	std::string m_folderDirectory;
	// List of material pointers for multiple textures
	std::vector< Material* > m_materials;
	// Map the indices based on key input e.g '5/1/1'
	std::map<std::string, unsigned int> m_vertexMap;

public:
	uint m_shaderID;

	// Constructors
	ObjectLoader();
	ObjectLoader( c_string a_folderDir, c_string a_fileDir);
	ObjectLoader( c_string a_folderDir, c_string a_fileDir, c_string a_textureName);
	// Deconstructors
	~ObjectLoader();

	// Returns the mesh object
	inline ObjectData* GetMesh() const { return m_object; }
	// Returns the list of material pointers
	inline const std::vector<Material*>& GetMaterials() { return m_materials; }
	// Clean up unused data in initialisation process
	void CleanUp();
};