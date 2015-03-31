#version 430

in layout(location=0) vec4 vertPosition;

uniform mat4 LightMatrix;

void main()
{
	gl_Position = LightMatrix * vertPosition;
}