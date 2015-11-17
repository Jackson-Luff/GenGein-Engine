#version 430

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec2 vCoords;

uniform sampler2D reflTexture;

void main()
{
	vec4 refl = texture(reflTexture, vCoords);
	gl_FragColor = vec4(refl.rgb, 1.0);
}