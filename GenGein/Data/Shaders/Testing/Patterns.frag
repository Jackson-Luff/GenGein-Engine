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
	float elTime = time;
	int tess = 100;
	uint seed = uint(tess*cos(vCoords.x) + tess*tan(vCoords.y));
	seed = seed * uint(tess*sin(vCoords.x) + tess*tan(vCoords.y));
	seed = seed + uint(sin(elTime));

	// Randomise colours
	float r = 1.0/rand(seed++, 255);
	float g = 1.0/rand(seed++, 255);
	float b = 1.0/rand(seed++, 255);

	// Base colours
	float rr = r * sin(elTime) +  clamp(cos(seed + elTime), -1.0, -0.15) + sin(elTime + vCoords.x)-1;
	float gg = g * sin(elTime) +  clamp(sin(seed + elTime), -1.0, -0.15) + cos(elTime + vCoords.y)-1;
	float bb = b * sin(elTime) +  clamp(cos(seed + elTime), -1.0, -0.15) + clamp(-sin(elTime + vCoords.y)+1, -10, 0);

	// Positioning emitter
	float dist = length(  (vec3(sin(elTime), 0, cos(elTime)) * 4.0) - vPosition.xyz);

	// Random stuff
	dist = 25 * sin(time)/dist + sin(seed++ + elTime);

	vec3 outRGB = vec3(rr,gg,bb);
	outRGB = outRGB + pow(tan(elTime + dist + sin(time)), clamp(sin(elTime), -1.0, 0.25));

	gl_FragColor = vec4(outRGB,1.0);
}

/* Weird stuff
int tess = 100;
uint seed = uint(tess*cos(vCoords.x) + tess*tan(vCoords.y));
seed = seed * uint(tess*sin(vCoords.x) + tess*tan(vCoords.y));
seed = seed + uint(sin(elTime));

// Randomise colours
float r = 1.0/rand(seed++, 255);
float g = 1.0/rand(seed++, 255);
float b = 1.0/rand(seed++, 255);

// Base colours
float rr = r * sin(elTime) +  clamp(cos(seed + elTime), -1.0, -0.15) + sin(elTime + vCoords.x)-1;
float gg = g * sin(elTime) +  clamp(sin(seed + elTime), -1.0, -0.15) + cos(elTime + vCoords.y)-1;
float bb = b * sin(elTime) +  clamp(cos(seed + elTime), -1.0, -0.15) + clamp(-sin(elTime + vCoords.y)+1, -10, 0);

// Positioning emitter
float dist = length(vec3(5 * sin(elTime), 0, 5 * tan(elTime)) - vPosition.xyz);

// Random stuff
dist = 1.0/dist + sin(seed + elTime);

vec3 outRGB = vec3(rr,gg,bb);
outRGB = outRGB + pow(tan(elTime + dist), clamp(sin(elTime), -1.0, 0.25));

gl_FragColor = vec4(outRGB,1.0);
*/