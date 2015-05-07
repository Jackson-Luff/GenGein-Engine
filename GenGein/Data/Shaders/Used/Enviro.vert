#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec2 vertNormal;
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
	vec4 Position = vertPosition;
	Position.y += (texture(heightMap, vertCoords).r * 2);
	gl_Position = Projection * View * Position;

	vPosition = Position;
	vNormals = vertNormal;
	vShadowCoords = LightMatrix * vertPosition;
	vCoords = vertCoords;
	
}
