#version 430

in layout(location=0) vec3 vertPosition;
in layout(location=1) vec3 vertColour;
in layout(location=2) vec3 vertNormal;
in layout(location=3) vec2 textureCoords;

uniform mat4 ProjectionView;

out vec2 tCoords;

void main()
{	
	gl_Position = ProjectionView * vec4(vertPosition, 1.0);
	tCoords = textureCoords;
}