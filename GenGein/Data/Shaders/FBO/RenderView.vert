#version 410

layout(location=0) in vec4 vertPosition;
layout(location=1) in vec2 vertTexCoord;

out vec2 vTexCoord;

void main()
{
	gl_Position = vertPosition;
	vTexCoord = vertTexCoord;
}
