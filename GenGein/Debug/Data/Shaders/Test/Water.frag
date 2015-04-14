#version 430

in vec4 vPosition;
in vec2 vCoords;

uniform sampler2D heightMap;
uniform sampler2D water;
uniform mat4 World;
uniform float time;

out vec4 pixelColour;

void main()
{
	vec4 fogColour = vec4(0.76,0.80,0.85, 1.0);

	vec4 diffuse = texture(water, vCoords);
	vec4 height  = texture(heightMap, vCoords);

	float camdist = length( World[3].xyz - vPosition.xyz );
	float fogMin = 400.0,fogMax = 500.0;

	pixelColour =  diffuse;//mix( diffuse, fogColour, camdist / fogMax );
	pixelColour.a = height.r;
	//TODO: Apply mix between high (white) and low(transparent)
}
