#version 430

in vec4 vPosition;
in vec4 vNormals;
in vec4 vShadowCoords;
in vec2 vCoords;

uniform mat4 World;
uniform vec3 AmbientLight;
uniform vec3 SunPos;

uniform sampler2D SandMap;
uniform sampler2D GrassMap;
uniform sampler2D StoneMap;

uniform float time;

out vec4 pixelColour;

float waterHeight = 10.0;

void main()
{
	// Sampling the textures
	vec4 outColour;
	vec4 sand = mix(texture(SandMap, vCoords*64), vec4(1), vPosition.y/10);
	vec4 grass = texture(GrassMap, vCoords*8);
	vec4 dirt = texture(StoneMap, vCoords*8);
	
	// Setting up the dot between normals and the up-vector
	float dotNU = max(0,dot(normalize(vNormals.xyz), vec3(0,1,0)) );
	
	// Diffused Light Calc's
	vec3 lightVector = normalize(SunPos - vPosition.xyz);
	float diffLight = max(0,dot(lightVector, normalize(vNormals.xyz) ));
	// -- 
	
	if(vPosition.y < waterHeight)
		outColour = mix(sand,mix(dirt, grass, dotNU), vPosition.y/10);
	else
		outColour = mix(dirt, grass, dotNU);
	
	if(SunPos.y > 0)
		outColour.rgb *= diffLight;
	else
		outColour.rgb *= AmbientLight;
		
	pixelColour = outColour;
}
