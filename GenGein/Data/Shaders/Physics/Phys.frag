#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
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

float ra(in uint seed)
{
	return rand(seed++, 2) -1;
}
void main()
{
	// Specular Light Calc's
	vec3 lightVector = normalize(vec3(15,20,20) - vPosition.xyz);
	vec3 reflectedLightVec = reflect(-lightVector, vNormal);
	vec3 eyeVector = normalize(vec3(-60, 60, -60)- vPosition.xyz);
	float specularity = max(0, dot(reflectedLightVec, eyeVector));
	specularity = pow(specularity, 5.0);
	
	vec3 outRGB = vec3(specularity);
	
	int x = int(vCoords.x * 100);
	int y = int(vCoords.y * 100);
	
	if(x % 10 == 0 || y % 10 == 0)
		outRGB = vec3(1.0,0.078,0.5764);
	
	gl_FragColor = vec4(outRGB, 1.0);
}
/* Another cool thing
vec3 spawn = vec3(0,time,0) * 2.5f;
	float dist = length(spawn - vPosition.xyz);
	uint seed = uint(time) + uint(dist);
	
	float r = ra(seed++);
	float g = ra(seed++);
	float b = ra(seed++);
	
	vec3 outRGB = vec3(0);
	
	outRGB.r = outRGB.r + vCoords.x + sin(r * time);
	outRGB.g = outRGB.g + vCoords.y + cos(g * time);
	outRGB.b = outRGB.b + tan(b * time);
	
	outRGB = outRGB + (ra(seed++) + time/seed)/ tan(dist);
	outRGB = outRGB + (ra(seed++) + time/seed)/ sin(vPosition.x);
	outRGB = outRGB + (ra(seed++) + time/seed)/ cos(vPosition.z);
*/

/* Pretty Awesome visual of random

vec3 spawn = vec3(0,time,0) * 5.0;
	float dist = length(spawn - vPosition.xyz);
	uint seed = uint(time) + uint(dist);
	
	float r = rand(seed++, 2) - 1;
	float g = rand(seed++, 2) - 1;
	float b = rand(seed++, 2) - 1;
	
	vec3 outRGB = vec3(0);
	
	outRGB.r = outRGB.r + vCoords.x + sin(r * time);
	outRGB.g = outRGB.g + vCoords.y + cos(g * time);
	outRGB.b = outRGB.b + tan(b * time);
	
	outRGB = outRGB + (r(seed++) + time/seed)* tan(dist);
	outRGB = outRGB + (r(seed++) + time/seed)* sin(vPosition.x);
	outRGB = outRGB + (r(seed++) + time/seed)* cos(vPosition.z);
	
*/

/* Cool line effect

vec3 spawn = vec3(0,time,0) * 5.0;
	float dist = length(spawn - vPosition.xyz);
	
	uint seed = uint(dist);
	seed = seed * uint(sin( vCoords.x * time));
	seed = seed * uint(sin( vCoords.y * time));	                   

	float r = rand(seed++, 2) - 1;
	float g = rand(seed++, 2) - 1;
	float b = rand(seed++, 2) - 1;
	
	vec3 outRGB = vec3(0);
	
	outRGB.r = outRGB.r + vCoords.x + sin(r * time);
	outRGB.g = outRGB.g + vCoords.y + cos(g * time);
	outRGB.b = outRGB.b + tan(b * time);
	
	outRGB = outRGB + g * sin(vPosition.x);
	outRGB = outRGB + b * cos(vPosition.z);
	*/