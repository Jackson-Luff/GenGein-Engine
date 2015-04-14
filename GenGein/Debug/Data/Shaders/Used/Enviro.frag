#version 430

in vec4 vPosition;
in vec4 vNormals;
in vec4 vShadowCoords;
in vec2 vCoords;

uniform mat4 World;

uniform sampler2D SandMap;
uniform sampler2D GrassMap;
uniform sampler2D StoneMap;

uniform float time;

out vec4 pixelColour;

float waterHeight = 10.0;

void main()
{
	vec3 sunPos = vec3(0, 500.0 * cos(time*0.05), 500.0 * sin(time*0.05));
	
	// Sampling the textures
	vec4 outColour;
	vec4 sand = mix(texture(SandMap, vCoords*64), vec4(1), vPosition.y/10);
	vec4 grass = texture(GrassMap, vCoords*8);
	vec4 dirt = texture(StoneMap, vCoords*8);
	
	// Setting up the dot between normals and the up-vector
	float dotNU = dot(normalize(vNormals.xyz), vec3(0,1,0));
	
	// Diffused Light Calc's
	vec3 lightVector = normalize(sunPos - vPosition.xyz);
	float diffLight = dot(lightVector, normalize(vNormals.xyz) );
	// -- 
	
	if(vPosition.y < waterHeight)
		outColour = mix(sand,mix(dirt, grass, dotNU), vPosition.y/10);
	else
		outColour = mix(dirt, grass, dotNU);
	
	if(sunPos.y > 0)
		outColour.rgb *= diffLight;
	else
		outColour.rgb = 1- vec3(vNormals.yyy);
		
	pixelColour = outColour;
}
