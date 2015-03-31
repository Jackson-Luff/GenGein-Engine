#version 410

in vec4 vColour;
in vec2 vCoords;
out vec4 fragColour;

uniform sampler2D pTexture;

void main()
{
	//fragColour = vColour;
	fragColour = vColour * texture(pTexture, vCoords);
}