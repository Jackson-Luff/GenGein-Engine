#version 430

layout(location=0) in vec3 Position;

out vec3 vCoords;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	gl_Position = Projection * mat4(mat3(View)) * vec4(Position,1.0);
	vCoords = Position;
}
