#version 410

// Input is points but output a quad
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Input data from vertex shader
in vec3 vPosition[];
in vec3 vColour[];
in vec3 vNormal[];
in vec2 vUv[];

// Output to fragment shader
out vec4 vColour;

uniform float totalWidth;
uniform float totalHeight;

// Called per-frame
uniform mat4 projectionView;
uniform mat4 cameraTransform;

void main()
{

	int w = 1, h = 1;
	vec3 corners[4];
	corners[0] = vec3( +w, 0, -h);
	corners[1] = vec3( +w, 0, +h);
	corners[2] = vec3( -w, 0, -h);
	corners[3] = vec3( -w, 0, +h);

	// Emit the front vertices for the quad
	gl_Position = projectionView*vec4(corners[0]+vPosition[0], 1);
	EmitVertex();
	gl_Position = projectionView*vec4(corners[1]+vPosition[1], 1);
	EmitVertex();
	gl_Position = projectionView*vec4(corners[2]+vPosition[2], 1);
	EmitVertex();
	gl_Position = projectionView*vec4(corners[3]+vPosition[3], 1);
	EmitVertex();
}
