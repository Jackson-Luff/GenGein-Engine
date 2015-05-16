#version 410

// Input is points but output a quad
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Input data from vertex shader
in vec3 vPosition[];
in float vLifetime[];
in float vLifespan[];

// Output to fragment shader
out vec4 vColour;
out vec2 vCoords;

uniform float sizeStart;
uniform float sizeEnd;
uniform vec4 colourStart;
uniform vec4 colourEnd;

// Called per-frame
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;
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
	// Interp colour
	vColour = mix( colourStart, colourEnd, vLifetime[0] / vLifespan[0] );
	
	// Calculate the size and create the corners of a quad
	float halfSize = mix(sizeStart, sizeEnd, vLifetime[0] / vLifespan[0] ) * 0.5f;
	
	vec3 corners[4];
	corners[0] = vec3( +halfSize, -halfSize, halfSize);
	corners[1] = vec3( +halfSize, +halfSize, halfSize);
	corners[2] = vec3( -halfSize, -halfSize, halfSize);
	corners[3] = vec3( -halfSize, +halfSize, halfSize);
	
	// Convert the input billboard data
	vec3 zAxis = normalize( World[3].xyz - vPosition[0] );
	vec3 xAxis = cross( World[1].xyz, zAxis );
	vec3 yAxis = cross( zAxis, xAxis );
	mat3 billboard = mat3(xAxis, yAxis, zAxis);
	
	// Emit the front vertices for the quad
	vCoords = vec2(1,0);
	gl_Position = Projection*View*vec4(billboard*corners[0]+vPosition[0], 1);
	EmitVertex();
	vCoords = vec2(1,1);
	gl_Position = Projection*View*vec4(billboard*corners[1]+vPosition[0], 1);
	EmitVertex();       
	vCoords = vec2(0,0);	
	gl_Position = Projection*View*vec4(billboard*corners[2]+vPosition[0], 1);
	EmitVertex(); 
	vCoords = vec2(0,1);	
	gl_Position = Projection*View*vec4(billboard*corners[3]+vPosition[0], 1);
	EmitVertex();
}