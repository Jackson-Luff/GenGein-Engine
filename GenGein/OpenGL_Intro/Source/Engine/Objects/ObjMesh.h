#pragma once
#include "BaseObject.h"

class ObjectLoader;

typedef unsigned int uint;
typedef const char* c_charp;

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
	ObjMesh(vec3 a_position);
	// Deconstructor
	~ObjMesh();
	
	// Load up the obj
	void LoadObject( c_charp a_folderDir, c_charp a_fileDir);
	void LoadObject( c_charp a_folderDir, c_charp a_fileDir, c_charp a_textureName);
	
	// Draw the mesh to screen
	void Render();
private:
	//Calculates the tangent of the vertex
	void CalcTangentNBiNormals();
	// Returns size of index
	inline const int GetIndexSize() const { return m_indexCount; }
	// Apply the data from the obj to the buffers
	void ApplyDataToVertNIndexBuffers(ObjectLoader* a_obj);

	// Mesh container
	ObjectLoader* m_obj;
};