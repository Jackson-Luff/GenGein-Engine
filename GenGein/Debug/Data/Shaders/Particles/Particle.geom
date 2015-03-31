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
uniform sampler2D diffuseMap;

// Called per-frame
uniform mat4 projectionView;
uniform mat4 cameraTransform;
uniform vec4 colourStrtOvrRid;
uniform vec4 colourEndOvrRid;

void main()
{
	// Interp colour
	vColour = mix( colourStrtOvrRid, colourEndOvrRid, vLifetime[0] / vLifespan[0] );
	
	// Calculate the size and create the corners of a quad
	float halfSize = mix(sizeStart, sizeEnd, vLifetime[0] / vLifespan[0] ) * 0.5f;
	
	vec3 corners[4];
	corners[0] = vec3( +halfSize, -halfSize, halfSize);
	corners[1] = vec3( +halfSize, +halfSize, halfSize);
	corners[2] = vec3( -halfSize, -halfSize, halfSize);
	corners[3] = vec3( -halfSize, +halfSize, halfSize);
	
	// Convert the input input billboard data
	vec3 zAxis = normalize( cameraTransform[3].xyz - vPosition[0] );
	vec3 xAxis = cross( cameraTransform[1].xyz, zAxis );
	vec3 yAxis = cross( zAxis, xAxis );
	mat3 billboard = mat3(xAxis, yAxis, zAxis);
	
	//billboard += sin(vLifetime[0] / vLifespan[0]);
	
	// Emit the front vertices for the quad
	vCoords = vec2(1,0);
	gl_Position = projectionView*vec4(billboard*corners[0]+vPosition[0], 1);
	EmitVertex();
	vCoords = vec2(1,1);
	gl_Position = projectionView*vec4(billboard*corners[1]+vPosition[0], 1);
	EmitVertex();       
	vCoords = vec2(0,0);	
	gl_Position = projectionView*vec4(billboard*corners[2]+vPosition[0], 1);
	EmitVertex(); 
	vCoords = vec2(0,1);	
	gl_Position = projectionView*vec4(billboard*corners[3]+vPosition[0], 1);
	EmitVertex();
}