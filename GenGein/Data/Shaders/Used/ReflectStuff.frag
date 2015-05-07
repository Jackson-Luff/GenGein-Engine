#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
in vec4 vIndices;
in vec4 vWeight;
in vec2 vCoords;

uniform mat4 World;
uniform vec3 SunPos;
uniform vec3 ambientLight = vec3(0.25, 0.25, 0.25);
uniform float time;

//Textures
uniform sampler2D reflectTexture;

uniform vec4 LightDiffuseColour = vec4(1, 1, 1, 1);
uniform vec4 LightAmbientColour = vec4(1, 1, 1, 1);
uniform vec4 LightSpecularColour = vec4(0.3, 0.3, 0.3, 1);

uniform float LightSpecularPower = 5.0;

out vec4 pixelColour;

void main()
{
	// =========== LIGHTING ============
	// Diffused Light Calc's
	//vec3 lightVector = normalize(SunPos - vPosition.xyz);
	//float brightness = dot(lightVector, normalize(vNormal) );

	// Specular Light Calc's
	//vec3 reflectedLightVec = reflect(-lightVector, vNormal);
	//vec3 eyeVector = normalize(World[3].xyz - vPosition.xyz);
	//float specularity = dot(reflectedLightVec, eyeVector);
	//specularity = pow(specularity, LightSpecularPower);

	// ============ POLISH ==============
	float r = mix(0, 1, normalize(vPosition.x));
	float g = mix(0, 1, normalize(vPosition.y));
	float b = mix(0, 1, normalize(vPosition.z));
	
	vec4 outcolour = vec4(r,g,b,1);
	
	//if(SunPos.y > -5)
	//	outcolour.rgb *= brightness;
	//else
	//	outcolour.rgb *= vec3(0.1);
		
	pixelColour = outcolour;
}