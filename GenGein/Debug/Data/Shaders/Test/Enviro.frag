#version 430

in vec4 vPosition;
in vec4 vNormals;
in vec4 vShadowCoords;
in vec2 vCoords;

uniform mat4 World;
uniform vec3 lightDir;
uniform sampler2D SandMap;
uniform sampler2D GrassMap;

uniform float time;

out vec4 pixelColour;

float waterHeight = 10.0;
void main()
{
	vec3 eyePos = World[3].xyz;

	vec4 outColour;
	vec4 sand = texture(SandMap, vCoords*64/1);
	vec4 grass = texture(GrassMap, vCoords*4/1);
	
	if(vPosition.y < waterHeight)
		outColour = mix(sand,grass, vPosition.y/10);
	else
		outColour = grass;
	
	//float d = max(0, dot(vNormals.xyz, World[3].xyz));
	
	pixelColour = outColour;// * d;
}
