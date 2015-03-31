#version 410

in vec4 vPosition;
in vec4 vColour;

out vec4 pixelColour;

void main()
{
	pixelColour = vColour;
}