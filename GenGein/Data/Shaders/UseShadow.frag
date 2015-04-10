#version 430

in vec4 vNormal;
in vec4 vShadowCoord;

out vec4 pixelColour;

uniform vec3 lightDir;
uniform sampler2D shadowMap;
uniform float shadowBias = 0.01;

void main() {

	float d = max(0, dot(normalize(vNormal.xyz), lightDir));

	if(texture(shadowMap, vShadowCoord.xy).r <
		vShadowCoord.z - shadowBias) {
		d = 0;
	}
	
	pixelColour = vec4(d, d, d, 1);
}
