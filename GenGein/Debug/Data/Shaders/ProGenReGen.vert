#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec2 vertCoords;

out vec4 vPosition;
out vec2 vCoords;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	gl_Position = Projection * View * vertPosition;
	vPosition = vertPosition;
	vCoords = vertCoords;
}
