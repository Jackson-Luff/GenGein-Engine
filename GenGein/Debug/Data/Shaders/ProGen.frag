#version 430

in vec2 vTexCoords;
out vec4 pixelColour;

uniform sampler2D perlin_texture;

void main()
{
	pixelColour = texture(perlin_texture, vTexCoords).rrrr;
	pixelColour.a = 1;
}
