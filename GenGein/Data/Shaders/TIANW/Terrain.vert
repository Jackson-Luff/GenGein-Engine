#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertNormal;
in layout(location=2) vec4 vertTangent;
in layout(location=3) vec2 vertCoords;

uniform mat4 Projection 	= mat4(1);
uniform mat4 View 			= mat4(1);
uniform mat4 LocalMatrix 	= mat4(1);
uniform mat4 World 			= mat4(1);

uniform sampler2D bumpyPerlinTexture;
uniform sampler2D smoothPerlinTexture;
uniform float MapHeightStrength = 1.0;
uniform float GeneralMapScale = 1.0;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiNormal;
out vec2 vCoords;

void main()
{
	//float bumpySample  = texture(bumpyPerlinTexture, vertCoords).r;
	//float smoothSample = texture(smoothPerlinTexture, vertCoords).r;
	
	vPosition = vertPosition.xyz;
	vNormal = vertNormal.xyz;
	vTangent = vertTangent.xyz;
	vBiNormal = cross(vNormal, vTangent) * sign(vertTangent.w);
	vCoords = vertCoords;
	
	gl_Position = Projection * View * LocalMatrix * vertPosition;
}
