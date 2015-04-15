#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
in vec4 vIndices;
in vec4 vWeight;
in vec2 vCoords;
out vec4 pixelColour;

uniform mat4 World;
uniform vec3 SunPos;
uniform vec3 AmbientLight;
uniform float time;

//Textures
uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D glowMap;
uniform sampler2D specularMap;
uniform sampler2D glossMap;
uniform sampler2D emissiveMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;
uniform sampler2D displacementMap;
uniform sampler2D decalMap;
uniform float LightSpecularPower = 5.0;

void main()
{
	// =========== LIGHTING ============
	// Diffused Light Calc's
	vec3 lightVector = normalize(SunPos - vPosition.xyz);
	float brightness = max(0,dot(lightVector, normalize(vNormal) ));

	// Specular Light Calc's
	vec3 reflectedLightVec = reflect(-lightVector, vNormal);
	vec3 eyeVector = normalize(World[3].xyz - vPosition.xyz);
	float specularity = max(0,dot(reflectedLightVec, eyeVector));
	specularity = pow(specularity, LightSpecularPower);

	// ============ POLISH ==============
	vec4 outcolour = texture(diffuseMap, vCoords);
	
	if(SunPos.y > -5)
		outcolour.rgb *= brightness;
	else
		outcolour.rgb *= AmbientLight;
		
	pixelColour = outcolour;
}