#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vPosition[];
out vec4 vNormals;

uniform mat4 Projection;
uniform mat4 View;
uniform int colOffset = 192;
void main()
{
	vec3 A = vPosition[2] - vPosition[0];
	vec3 B = vPosition[1] - vPosition[0];
	vNormals = normalize(cross(A, B));
	
	gl_Position = Projection * View * vPosition[0];
    EmitVertex();
    gl_Position = Projection * View * vPosition[1];
    EmitVertex();
    gl_Position = Projection * View * vPosition[2];
    EmitVertex();
    EndPrimitive();
}*/