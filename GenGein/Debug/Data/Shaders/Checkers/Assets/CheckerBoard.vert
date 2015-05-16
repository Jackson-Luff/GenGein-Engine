#version 430

in layout(location=0) vec3 in_position;
in layout(location=1) vec2 in_uv;

out vec2 vCoords;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	gl_Position = Projection * View * vec4(in_position,1);
	vCoords = in_uv;
}