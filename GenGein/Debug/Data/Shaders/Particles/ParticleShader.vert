#version 410

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertColour;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;

uniform float time;

out vec4 vPosition;
out vec4 vColour;

void main()
{
	vPosition = vertPosition;
	vColour = vertColour;

	gl_Position = Projection * View * vPosition;
}