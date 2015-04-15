#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertNormals;
in layout(location=2) vec2 vertCoords;

out vec4 vPosition;
out vec4 vNormals;
out vec4 vShadowCoords;
out vec2 vCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 LightMatrix;
uniform float time;

uniform sampler2D heightMap;

void main()
{
	gl_Position = Projection * View * vertPosition;

	vPosition = vertPosition;
	vNormals = vertNormals;
	vCoords = vertCoords;
	
}
