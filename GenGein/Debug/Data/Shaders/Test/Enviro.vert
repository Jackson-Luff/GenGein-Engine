#version 430

in layout(location=0) vec3 vertPosition;
in layout(location=2) vec3 vertNormal;
in layout(location=3) vec2 vertCoords;

out vec4 vPosition;
out vec2 vCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform float time;

uniform sampler2D heightMap;

void main()
{
	vec4 Position = vec4(vertPosition,1.0);
	vec4 texel = texture(heightMap, vertCoords);
	Position.y = texel.y * 50;

	gl_Position = Projection * View * Position;

	vPosition = Position;
	vCoords = vertCoords;
}
