#version 430

in vec2 vCoords;
uniform float tileCount = 8.0;

void main()
{
	float c;
	
	c = floor(vCoords.x*tileCount);
	c = c * 1-floor(vCoords.y*tileCount);
	c = mod(c,2);
	
	gl_FragColor =  vec4(c, c, c, 1);
}