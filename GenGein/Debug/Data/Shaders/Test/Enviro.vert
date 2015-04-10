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
	vec4 Position = vertPosition;
	//Position.y += -100/2 + (texture(heightMap, vertCoords).r * 100);
	gl_Position = Projection * View * Position;

	vPosition = Position;
	vNormals = vertNormals;
	vShadowCoords = LightMatrix * vertPosition;
	vCoords = vertCoords;
	
}
