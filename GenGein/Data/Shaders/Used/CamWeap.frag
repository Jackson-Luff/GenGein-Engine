#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec2 vCoords;

// Lighting Attributes
uniform vec3 SunPos;
uniform vec3 AmbientLight;

//Textures
uniform sampler2D diffuseMap;

uniform float LightSpecularPower = 5.0;

out vec4 pixelColour;

void main()
{
	// Diffused Light Calc's
	vec3 lightVector = normalize(SunPos - vPosition.xyz);
	float brightness = max(0,dot(lightVector, normalize(vNormal) ));
	
	vec4 outColour = texture(diffuseMap, vCoords);
	
	if(SunPos.y > 0)
		outColour.rgb *= brightness;
	else
		outColour.rgb *= AmbientLight;
	
	pixelColour = outColour;
}