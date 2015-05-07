#version 410

in vec2 vTexCoord;
out vec4 FragColour;

uniform sampler2D diffuse;
uniform float time;

void main()
{ 
	vec2 texelSize = time / textureSize( diffuse, vTexCoord );
	gl_FragColor = texture( diffuse,  vTexCoord );
}