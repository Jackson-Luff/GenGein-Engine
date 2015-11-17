#version 430

in vec3 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
in vec2 vCoords;

uniform mat4 World = mat4(1.0);
uniform vec3 lightPos = vec3(100.0);
uniform float elapsedTime = -1.0;

float DiffuseLightPower = 5.0;
vec3 DiffuseLightColour = vec3(1);
float SpecularLightPower = 5.0;
vec3 SpecularLightColour = vec3(1, 0.75, 0.65);
float VCoordScale = 50.0;

uniform sampler2D grassDiffuse;
uniform sampler2D grassNormal;
uniform sampler2D stoneDiffuse;
uniform sampler2D stoneNormal;

uniform sampler2D bumpyPerlinTexture;
uniform sampler2D smoothPerlinTexture;

out vec4 pixelColour;

vec3 DiffuseSplotch(in sampler2D t, in vec2 coords, in float baseCoordMulti,
  in float lCoordMulti, in float lAlter)
{
	/* Setting up the the grass\'s diffuse*/
	// Setup textures with varied tile samples
	vec3 data  = texture(t, coords*baseCoordMulti).rgb;
	vec3 firstOverlay   = texture(t, coords*lCoordMulti).rgb;
	vec3 secondOverlay  = texture(t, coords*20.0).rgb;
   
	// Check for light splotching of green
	if(firstOverlay.g > 0.2)
			data += firstOverlay + lAlter;

	// Check for heavy splotching of blue
	if(secondOverlay.g > 0.6)
			data += secondOverlay * vec3(0.7);
	
	data = mix(data, data*vec3(0.8), 1);
	
	return data;
}
 
vec3 NormalSplotch(in sampler2D t, in vec2 coords, in float baseCoordMulti,
 in float lSplotch, in float lCoordMulti,
 in float hSplotch, in float hCoordMulti)
{
    /* Setting up the the grass\'s diffuse*/
	// Setup textures with varied tile samples
	vec3 data  = texture(t, coords*baseCoordMulti).rgb * 2 - 1;
	vec3 firstOverlay   = texture(t, coords*lCoordMulti).rgb * 2 - 1;
	vec3 secondOverlay  = texture(t, coords*hCoordMulti).rgb * 2 - 1;
	
	// Check for light splotching of green
	if(firstOverlay.g > lSplotch)
			data = normalize(data + firstOverlay);
		
	// Check for heavy splotching of green
	if(secondOverlay.b > hSplotch)
			data = normalize(data + secondOverlay);
	return data;
}

void main()
{
	vec3 lightPosition = vec3(3000 + sin(elapsedTime), 5000, 3000);
	vec3 lightVector = normalize(lightPosition - vPosition);
	
	float dotNorm = dot(normalize(vNormal), vec3(0.0,1.0,0.0));
	dotNorm = smoothstep(0.8, 1.0, dotNorm * 0.5 + 0.5);
	
	mat3 TBN = mat3(
		normalize( vTangent ),
		normalize( vBiNormal ),
		normalize( vNormal ));
	
	vec3 grassNormalSamp = NormalSplotch(grassNormal, vCoords, VCoordScale, 0.2, 3, 0.6, 10);
	vec3 stoneNormalSamp = texture(stoneNormal, vCoords*VCoordScale).rgb * 2 -1;
	vec3 normalSampData = mix(stoneNormalSamp, grassNormalSamp, dotNorm);
	float dNLight = max( 0, dot( TBN * normalSampData, lightVector ));
	vec4 diffLight = vec4(vec3(pow(max( 0, dot( vNormal, lightVector )), 2.0)), 1.0);
	
	vec3 grassDiffuseSamp = DiffuseSplotch(grassDiffuse, vCoords, VCoordScale, 3, -0.2);
	vec3 stoneDiffuseSamp = DiffuseSplotch(stoneDiffuse, vCoords, VCoordScale, 2, +0.0) * 0.85;
	
	vec3 diffuseSampData = mix(stoneDiffuseSamp, grassDiffuseSamp, dotNorm);	// =========== EVEN MORE FANCY STUFF =======

	pixelColour = vec4(diffuseSampData * dNLight, 1.0) * clamp(diffLight, 0.2, 1.0);
}
