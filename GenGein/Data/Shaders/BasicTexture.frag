#version 430

in vec4 vColour;
in vec2 tCoords;

out vec4 pixelColour;

uniform sampler2D diffuseMap;

void main()
{
	pixelColour = texture(diffuseMap, tCoords);
}