#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertNormal;
in layout(location=3) vec2 vertCoords;

out vec4 vPosition;
out vec4 vNormal;
out vec2 vCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform float elapsedTime;

void main()
{
	vec4 vPos = vertPosition;
	
	vPos.y += 1800;
	
	vPos.y += 7 * cos(elapsedTime + vPos.x) * sin( elapsedTime + vPos.z ) * 5;
	vPos.z += sin(elapsedTime);
	
	gl_Position = Projection * View * vPos;
	vPosition = vertPosition;
	vNormal = vertNormal;
	vCoords = vertCoords;
}
