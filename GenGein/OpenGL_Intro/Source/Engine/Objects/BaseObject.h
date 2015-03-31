#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

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
	uint m_indexCount;

	uint m_VAO, m_VBO, m_IBO;
	uint m_programID;

	mat4 m_localTransform;
	mat4 m_globalTransform;
};

