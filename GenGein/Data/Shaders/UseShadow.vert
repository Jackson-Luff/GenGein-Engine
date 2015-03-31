#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=2) vec4 vertNormal;

out vec4 vNormal;
out vec4 vShadowCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 LightMatrix;

void main() {
	vNormal = vertNormal;
	gl_Position = Projection * View * vertPosition;
	
	vShadowCoord = LightMatrix * vertPosition;
}