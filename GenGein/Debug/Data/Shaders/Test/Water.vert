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
	vec4 texel = texture(heightMap, vertCoords);
	vec4 Position = vec4(vertPosition,1.0);

	Position.y = 10 + (texel.y * 5);

	/*
	Position.y += cos(time + Position.x) * sin( time + Position.z ) * 0.1;
	Position.z += 0.5 * sin(time);*/

	gl_Position = Projection * View * Position;

	vPosition = Position;
	vCoords = vertCoords;
}
