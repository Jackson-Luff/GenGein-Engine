#version 410

// Input is points but output a quad
layout(triangle_strip, max_vertices = 3) in;
layout(triangle_strip, max_vertices = 3) out;

// Input data from vertex shader
in vec3 vPosition[];
in vec3 vNormal[];

void main()
{
	// => pickout vPosition[0][1][2]
	// => calculate persistant normal
	// => apply and emit
	
	EmitVertex();
}