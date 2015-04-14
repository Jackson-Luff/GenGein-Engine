#pragma once
#include <string>
#include "BaseObject.h"

class ObjMesh;

typedef unsigned int uint;
typedef const char* c_charp;
typedef const std::string c_string;

////
// Author: Jackson Luff
// Name: ObjMesh
// Type: class (standard)
// Parent: None
// Description:
// * ObjMesh will contain hold the vertex 
// * info used on the VBO, as well as the
// * indices info for the IBO. Another
// * element of this class is a an object mesh
// * refence that has multiple uses.
////
class ObjMesh : public BaseObject
{
public:
	// Constructors
	ObjMesh();
	// Deconstructor
	~ObjMesh();
	
	// Load up the obj
	void LoadObject( c_charp a_folderDir, c_charp a_fileDir);
	void LoadObject( c_charp a_folderDir, c_charp a_fileDir, c_charp a_textureName);
	
	// Draw the mesh to screen
	void Render();

	// Getter for localMatrix
private:

	// Holds most attributes of a material
	struct Material
	{
		std::string name;
		glm::vec3 Ka, Kd, Ks;
		float d, Ns, illum;
	};

	//Calculates the tangent of the vertex
	void CalcTangentNBiNormals();
	// Returns size of index
	inline const int GetIndexSize() const { return m_indexCount; }
	// Apply the data from the obj to the buffers
	void ApplyDataToVertNIndexBuffers();

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

	// Returns the list of material pointers
	inline const std::vector<Material*>& GetMaterials() { return m_materials; }
	// Clean up unused data in initialisation process
	void CleanUp();

	// Points to current material used
	Material* m_currentMaterial;
	// Points to completed mesh
	Temp_OBJ_Data m_object;
	// Determines as to whether the texture has these traits
	bool m_hasUV, m_hasNormals, m_isQuads, m_indexChecked;
	// Points to file directory
	std::string m_folderDirectory;
	// List of material pointers for multiple Renderer
	std::vector< Material* > m_materials;
	// Map the indices based on key input e.g '5/1/1'
	std::map<std::string, unsigned int> m_vertexMap;
};