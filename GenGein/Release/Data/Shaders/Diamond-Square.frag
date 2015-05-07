#version 430

in vec2 vCoords;
out vec4 pixelColour;

uniform sampler2D perlin_texture;

void main()
{
	pixelColour = texture(perlin_texture, vCoords);
}
