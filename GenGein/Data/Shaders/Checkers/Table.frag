#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
in vec4 vIndices;
in vec4 vWeight;
in vec2 vCoords;

// Lighting Attributes
uniform float time;
uniform vec3 eyePos;
uniform vec3 SunPos = vec3(0);
uniform vec3 ambientLight = vec3(0.25, 0.25, 0.25);

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

uniform vec4 LightDiffuseColour = vec4(1, 1, 1, 1);
uniform vec4 LightAmbientColour = vec4(1, 1, 1, 1);
uniform vec4 LightSpecularColour = vec4(0.3, 0.3, 0.3, 1);

uniform float LightSpecularPower = 5.0;

out vec4 pixelColour;

void main()
{
	// =========== LIGHTING ============

	// Diffused Light Calc's
	vec3 lightVector = normalize(SunPos - vPosition.xyz);
	float brightness = pow(max(0,dot(lightVector, normalize(vNormal))), 1.0);
	// ============ POLISH ==============
	pixelColour = texture(diffuseMap, vCoords);
	pixelColour.rgb = pixelColour.rgb * (brightness);
}