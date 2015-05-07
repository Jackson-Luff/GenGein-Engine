#include <gl_core_4_4.h>
#include "Engine\Core\ShaderHandler.h"
#include "BaseObject.h"

BaseObject::BaseObject()
	: m_indexCount(0),
	m_VAO(0), m_VBO(0), m_IBO(0),
	m_programID(0),
	m_localTrans(mat4(1))
{}

BaseObject::~BaseObject()
{}

void ApplyOpenGLData()
{

}