#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertColour;
in layout(location=2) vec4 vertNormal;
in layout(location=3) vec4 vertTangent;
in layout(location=4) vec4 vertBiNormal;
in layout(location=5) vec2 vertTextCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 LocalMatrix;
uniform mat4 World;

uniform float time;

out vec4 vPosition;
out vec4 vColour;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiNormal;
out vec2 vCoords;

void main()
{
	vPosition = vertPosition;
	vColour = vertColour;
	vNormal = vertNormal.xyz;
	vTangent = vertTangent.xyz;
	vBiNormal = vertBiNormal.xyz;
	vCoords = vertTextCoords;

	gl_Position = Projection * View * LocalMatrix * vertPosition;
}