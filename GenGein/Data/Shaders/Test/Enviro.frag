#version 430

in vec4 vPosition;
in vec2 vCoords;

uniform mat4 World;
uniform sampler2D grassMap;
uniform sampler2D heightMap;

uniform float time;

out vec4 pixelColour;

void main()
{
	vec3 eyePos = World[3].xyz;
	vec4 fogColour = vec4(0.76,0.80,0.85, 1.0);

	vec4 grass = texture(grassMap, vCoords);

	float camdist = length( eyePos - vPosition.xyz );
	float fogMin = 400.0,fogMax = 500.0;

	vec4 outputColour = grass;//mix(grass, stone, vPosition.y/50);

	pixelColour = outputColour;//mix( grass, fogColour, camdist / fogMax );
}
