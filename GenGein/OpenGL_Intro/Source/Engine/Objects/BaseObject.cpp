#include "BaseObject.h"


BaseObject::BaseObject()
	: m_indexCount(0),
	m_VAO(0), m_VBO(0), m_IBO(0),
	m_programID(0),
	m_localTransform(mat4(1)),
	m_globalTransform(mat4(1))
{}

BaseObject::BaseObject(vec3 a_position)
{
	BaseObject();
}

BaseObject::~BaseObject()
{}
