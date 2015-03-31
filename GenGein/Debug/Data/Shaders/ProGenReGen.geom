#version 430

 layout(points) in;
 layout(points, max_vertices = 1) out;

 in vec4 vPosition[];

void main()
{
	vec4 pos = vPosition[0];
	pos.y *= c.r * 50;

	gl_Position = vPosition[0];
}
