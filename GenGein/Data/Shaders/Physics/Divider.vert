#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertColour;
in layout(location=2) vec4 vertNormal;
in layout(location=3) vec4 vertTangent;
in layout(location=4) vec4 vertBiNormal;
in layout(location=5) vec4 vertIndices;
in layout(location=6) vec4 vertWeight;
in layout(location=7) vec2 vertCoords;

out vec4 vPosition;
out vec4 vColour;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiNormal;
out vec4 vIndices;
out vec4 vWeight;
out vec2 vCoords;

flat out int vID;


uniform mat4 Projection;
uniform mat4 View;
uniform mat4 LocalMatrix;
uniform float time;

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
	vID = gl_VertexID;
	
	// case the indices to the integer's so they can index an array
	//ivec4 index = ivec4(vertIndices);

	//vec4 P = bones[ index.x ] * vertPosition * vertWeight.x;
	//P += bones[ index.y ] * vertPosition * vertWeight.y;
	//P += bones[ index.z ] * vertPosition * vertWeight.z;
	//P += bones[ index.w ] * vertPosition * vertWeight.w;
		
	gl_Position = Projection * View * LocalMatrix * vPosition;
}