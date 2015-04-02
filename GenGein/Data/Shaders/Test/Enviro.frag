#version 430

in vec4 vPosition;
in vec2 vCoords;

uniform mat4 World;
uniform sampler2D SandMap;
uniform sampler2D GrassMap;

uniform float time;

out vec4 pixelColour;

float waterHeight = 0.0;
void main()
{
	vec3 eyePos = World[3].xyz;

	float dist = length( eyePos - vPosition.xyz );
	float fogMin = 400.0,fogMax = 500.0;

	vec4 outColour;
	vec4 sand = texture(SandMap, vCoords*1024/1);
	vec4 grass = texture(GrassMap, vCoords*128/1);

	if(vPosition.y <= waterHeight+1)
		outColour = mix(sand,grass, vPosition.y);
	else
		outColour = grass;

	pixelColour = outColour;
}
