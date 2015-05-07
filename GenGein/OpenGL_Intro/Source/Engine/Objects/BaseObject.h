#pragma once
#include <FBXFile.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

typedef unsigned int uint;
typedef const unsigned int c_uint;

using glm::vec3;
using glm::vec4;
using glm::mat4;

class BaseObject
{
public:

	BaseObject();
	~BaseObject();

	// Getter / Setter of Position
	glm::vec4 const GetPosition() { return m_localTransform[3]; }
	void const SetPosition(vec3 newPos) { m_localTransform[3] = vec4(newPos,1); }

	// Getter / Setter of Rotation
	inline void SetRotation( const float a_angleInDegrees, const vec3& a_appOfRot)
		{ m_localTransform *= glm::rotate(a_angleInDegrees, a_appOfRot); }
	
	// Getter / Setter of Scale
	inline const void SetScale(float a_scaleValue)
		{m_localTransform *= glm::scale(vec3(a_scaleValue));}

	// Getter / Setter of LocalTransform
	inline glm::mat4 GetLocalTransform() const { return m_localTransform; }
	inline const void SetLocalTransform(const glm::mat4 a_locTrans)
	{ m_localTransform = a_locTrans; }

	inline const void SetLocalTransform(
		const glm::vec3& a_pos,
		const float& a_angle,
		const glm::vec3& a_rot,
		const glm::vec3& a_scale)
	{ 
		m_localTransform = glm::translate(a_pos) * 
			glm::rotate(a_angle, a_rot) * glm::scale(a_scale);
	}

	void ApplyOpenGLData();

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
	uint* m_programID;

	int m_localMatUniLoc;
	mat4 m_localTransform;

	std::vector< FBXVertex > m_FBX_verts;
	std::vector< OBJVertex > m_OBJ_verts;
	std::vector< uint > m_indices;
};

