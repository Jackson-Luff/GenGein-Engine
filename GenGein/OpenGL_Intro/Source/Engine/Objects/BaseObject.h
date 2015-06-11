#pragma once
#include <FBXFile.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

using namespace glm;

class BaseObject
{
public:

	BaseObject();
	~BaseObject();

	// Getter / Setter of Position
	const f32vec4& GetPosition() { return m_localTrans[3]; }
	void const SetPosition(const f32vec3& newPos) { m_localTrans[3] = f32vec4(newPos,1); }

	// Getter / Setter of Rotation
	inline const void SetRotation( const float32_t& a_angleInDegrees, const f32vec3& a_appOfRot)
		{ m_localTrans *= rotate(a_angleInDegrees, a_appOfRot); }
	
	// Getter / Setter of Scale
	inline const void SetScale(const float32_t& a_scaleValue)
		{m_localTrans *= scale(f32vec3(a_scaleValue));}

	// Getter / Setter of LocalTransform
	inline const f32mat4 GetLocalTransform() const 
		{ return m_localTrans; }
	inline const void SetLocalTransform(const f32mat4& a_locTrans)
		{ m_localTrans = a_locTrans; }

	inline const void SetLocalTransform(
		const f32vec3& a_pos,
		const float32_t& a_angle,
		const f32vec3& a_rot,
		const f32vec3& a_scale)
	{ m_localTrans = translate(a_pos) *  rotate(a_angle, a_rot) * scale(a_scale); }

	void ApplyOpenGLData();

protected:
	// Vertex holds the layout info for the VBO
	struct OBJVertex
	{
		f32vec4 position;
		f32vec4 colour;
		f32vec4 normal;
		f32vec4 tangent;
		f32vec4 binormal;
		f32vec2 uv;
	};
	
	struct Temp_OBJ_Data
	{
		std::vector<f32vec4> position, normals, colours;
		std::vector<f32vec2> texCoords;
		std::vector<OBJVertex> vertices;
		std::vector<uint32_t> indices;
	};

	uint32_t m_indexCount;

	uint32_t m_VAO, m_VBO, m_IBO;
	uint32_t* m_programID;

	int32_t m_localMatUniLoc;
	f32mat4 m_localTrans;

	std::vector< FBXVertex > m_FBX_verts;
	std::vector< OBJVertex > m_OBJ_verts;
	std::vector< uint32_t > m_indices;
};

