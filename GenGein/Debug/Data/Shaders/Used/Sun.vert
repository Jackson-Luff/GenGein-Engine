#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertColour;
in layout(location=2) vec4 vertNormal;
in layout(location=3) vec4 vertTangent;
in layout(location=4) vec4 vertBiNormal;
in layout(location=5) vec4 vertIndices;
in layout(location=6) vec4 vertWeight;
in layout(location=7) vec2 vertCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;
uniform mat4 LocalMatrix;
uniform float time;

out vec4 vPosition;
out vec4 vColour;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiNormal;
out vec4 vIndices;
out vec4 vWeight;
out vec2 vCoords;

// we need to give our bone array a limit
const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main()
{
	vPosition = vertPosition;
	vColour = vertColour;
	vNormal = vertNormal.xyz;
	vTangent = vertTangent.xyz;
	vBiNormal = vertBiNormal.xyz;
	vIndices = vertIndices;
	vWeight = vertWeight;
	vCoords = vertCoords;
	
	vec3 zAxis = normalize( World[3].xyz - vertPosition.xyz );
	vec3 xAxis = cross( World[1].xyz, zAxis );
	vec3 yAxis = cross( zAxis, xAxis );
	mat3 billboard = mat3(xAxis, yAxis, zAxis);
	
	gl_Position = Projection * View * LocalMatrix * (mat4(billboard) * vertPosition);
}
