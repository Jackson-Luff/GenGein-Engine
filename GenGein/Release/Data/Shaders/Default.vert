#version 430

in layout(location=0) vec3 vertPosition;
in layout(location=1) vec3 vertColour;
in layout(location=2) vec3 vertNormal;
in layout(location=3) vec2 vertCoords;

out vec4 vColour;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	gl_Position = Projection * View * Position;
	vColour = vec4(vertColour, 1.0);
}
