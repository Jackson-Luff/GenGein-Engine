#version 430

in vec4 vPosition;
in vec4 vColour;
in vec4 vNormal;
in vec4 vTangent;
in vec4 vBiNormal;
in vec4 vIndices;
in vec4 vWeight;
in vec2 vCoords;
flat in int vID;

uniform mat4 World;
uniform float time;
uniform vec3 SunPos;


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
	// =========== LIGHTING ============
	// Diffused Light Calc's
	vec3 lightVector = normalize(SunPos - vPosition.xyz);
	float brightness = dot(lightVector, normalize(vNormal.xyz) );
	
	vec3 outRGB = vec3(1);
	outRGB *= brightness;
	
	gl_FragColor = vec4(outRGB,1.0);
}
/*
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

gl_FragColor = vec4(outRGB,1.0);*/