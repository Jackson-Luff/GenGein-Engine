#pragma once
#include <FBXFile.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

typedef unsigned int uint;

using glm::vec3;
using glm::vec4;
using glm::mat4;

class BaseObject
{
public:
	BaseObject();
	BaseObject(vec3 a_position);
	~BaseObject();

	// Getter / Setter of Position
	vec4 const GetPosition() const { return m_localTransform[3]; }
	void const SetPosition(vec4 newPos) { m_localTransform[3] = newPos; }

	// Getter / Setter of Rotation
	inline void SetRotation( const float a_angleInDegrees, const vec3& a_appOfRot)
	{
		m_localTransform = glm::rotate(m_localTransform, a_angleInDegrees, a_appOfRot);
	}

	// Getter / Setter of Scale
	inline void const SetScale(float a_scaleValue) 
	{
		vec3 scale = vec3(a_scaleValue);
		m_localTransform = glm::scale(m_localTransform, scale);
	}

protected:
	// Vertex holds the layout info for the VBO
	struct OBJVertex
	{
		glm::vec4 position;
		glm::vec4 colour;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec4 binormal;
		glm::vec2 uv;
	};
	
	struct Temp_OBJ_Data
	{
		std::vector<glm::vec4> position, normals, colours;
		std::vector<glm::vec2> texCoords;
		std::vector<OBJVertex> vertices;
		std::vector<uint> indices;
	};

	uint m_indexCount;

	uint m_VAO, m_VBO, m_IBO;
	uint m_programID;

	mat4 m_localTransform;
	mat4 m_globalTransform;

	std::vector< FBXVertex > m_FBX_verts;
	std::vector< OBJVertex > m_OBJ_verts;
	std::vector< uint > m_indices;
};

