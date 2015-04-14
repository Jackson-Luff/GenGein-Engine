#version 430

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec2 texCoords;
out vec2 vTexCoords;

uniform sampler2D perlin_texture;

uniform mat4 Projection;
uniform mat4 View;

void main() {
	vec4 vPos = vertPosition;
	vPos.y += texture(perlin_texture, texCoords).r * 50;
	
	gl_Position = Projection * View * vPos;
	vTexCoords = texCoords;
}