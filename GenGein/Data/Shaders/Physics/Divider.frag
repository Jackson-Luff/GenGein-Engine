#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec2 vCoords;
flat in int vID;

uniform mat4 World;
uniform vec3 SunPos;
uniform float time;

const float INVERSE_MAX_UINT = 1.0 / 4294967295.0;

float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

float ra(uint seed)
{
	return rand(seed++, 2) - 1;
}
void main()
{
	float t = time * 100;
	
	if(int(t) % 100 == 2)
		t = pow(t,2);
	
	t = t / 100;
	
	int tess = 100;
	uint   seed = uint(tess*cos(vCoords.x) + tess*tan(vCoords.y));
	seed = seed * uint(tess*sin(vCoords.x) + tess*tan(vCoords.y));
	seed = seed + uint(sin(t));
	
	// Initialise
	vec3 outRGB = vec3(1);
	
	// Target
	vec3 target = vec3(sin(t*vCoords.x*10), 0, cos(t*vCoords.y*10));
	
	// Watchdogs like background
	target = target + (ra(seed++) / length(vCoords));
	
	float dist = length(target - vPosition.xyz);
	
	// Almost a smoke-like effect
	dist = dist - sin(seed - t);

	// Colouring
	outRGB = outRGB * vec3(0.1, 1.0, 1.0) * 1-dist;
		
	gl_FragColor = vec4(outRGB,0.5);
}