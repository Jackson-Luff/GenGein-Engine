#version 430

in vec3 vCoords;
out vec4 pixelColour;

uniform samplerCube skybox;

void main()
{
	pixelColour = texture(skybox, vCoords);
}