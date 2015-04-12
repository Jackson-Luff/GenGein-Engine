#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
in vec2 vCoords;

// Lighting Attributes
uniform vec3 eyePos;
uniform vec3 lightPos = vec3( 0.0f, 1.0f, 0.0f);
uniform vec3 ambientLight = vec3(0.25, 0.25, 0.25);

//Textures
uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D emissiveMap;
uniform sampler2D displacementMap;
uniform sampler2D decalMap;
uniform sampler2D bumpMap;

uniform vec4 LightDiffuseColour = vec4(1, 1, 1, 1);
uniform vec4 LightAmbientColour = vec4(1, 1, 1, 1);
uniform vec4 LightSpecularColour = vec4(0.3, 0.3, 0.3, 1);

uniform float LightSpecularPower = 10.0;

out vec4 pixelColour;

void main()
{

	// =========== LIGHTING ============

	// Diffused Light Calc's
	vec3 lightVector = normalize(lightPos - vPosition.xyz);
	float brightness = dot(lightVector, normalize(vNormal) );
	brightness = pow(brightness, 5.0);
	vec4 diffusedLight = vec4(brightness, brightness, brightness, 1.0);

	// Specular Light Calc's
	vec3 reflectedLightVec = reflect(-lightVector, vNormal);
	vec3 eyeVector = normalize(eyePos - vPosition.xyz);
	float specularity = dot(reflectedLightVec, eyeVector);
	specularity = pow(specularity, LightSpecularPower);
	vec4 specularLight = vec4(specularity, specularity, specularity, 1.0);

	// =========== TEXTURES ============

	mat3 TBN = mat3(
		normalize( vTangent ),
		normalize( vBiNormal ),
		normalize( vNormal ));

	vec3 N = texture(normalMap, vCoords).xyz * 2 - 1;
	vec3 S = texture(specularMap, vCoords).xyz * 2 - 1;
	float d = max( 0, dot( normalize( TBN * N), lightVector ));

	// ============ POLISH ==============
	pixelColour = texture(diffuseMap, vCoords);
	//pixelColour.rgb *= d;
}
