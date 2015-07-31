#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
in vec4 vIndices;
in vec4 vWeight;
in vec2 vCoords;
flat in int vID;

uniform mat4 World;
uniform vec3 SunPos;
uniform float time;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

void main()
{
   // Specular Light Calc's
	vec3 lightVector = normalize(vec3(-15,20,20) - vPosition.xyz);
	vec3 reflectedLightVec = reflect(-lightVector, vNormal);
	vec3 eyeVector = normalize(vec3(60, 60, -60)- vPosition.xyz);
	float specularity = max(0, dot(reflectedLightVec, eyeVector));
	specularity = pow(specularity, 5.0);
	
	vec3 outRGB = vec3(specularity);
	
	int x = int(vCoords.x * 100);
	int y = int(vCoords.y * 100);
	
	if(x % 10 == 2 || y % 10 == 2)
		outRGB = vec3(0.0, 1.0, 1.0);
	
	gl_FragColor = vec4(outRGB, 1.0);
}

/* Weird stuff
int tess = 100;
uint seed = uint(tess*cos(vCoords.x) + tess*tan(vCoords.y));
seed = seed * uint(tess*sin(vCoords.x) + tess*tan(vCoords.y));
seed = seed + uint(sin(time));

// Randomise colours
float r = 1.0/rand(seed++, 255);
float g = 1.0/rand(seed++, 255);
float b = 1.0/rand(seed++, 255);

// Base colours
float rr = r * sin(time) +  clamp(cos(seed + time), -1.0, -0.15) + sin(time + vCoords.x)-1;
float gg = g * sin(time) +  clamp(sin(seed + time), -1.0, -0.15) + cos(time + vCoords.y)-1;
float bb = b * sin(time) +  clamp(cos(seed + time), -1.0, -0.15) + clamp(-sin(time + vCoords.y)+1, -10, 0);

// Positioning emitter
float dist = length(vec3(5 * sin(time), 0, 5 * tan(time)) - vPosition.xyz);

// Random stuff
dist = 1.0/dist + sin(seed + time);

vec3 outRGB = vec3(rr,gg,bb);
outRGB = outRGB + pow(tan(time + dist), clamp(sin(time), -1.0, 0.25));

gl_FragColor = vec4(outRGB,1.0);
*/