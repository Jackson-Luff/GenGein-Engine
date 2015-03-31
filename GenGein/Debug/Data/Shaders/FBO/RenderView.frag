#version 410

in vec2 vTexCoord;
out vec4 pixelColour;

uniform sampler2D target;
uniform float time;

// AIE Interpretations
vec4 Simple()
{
	return texture(target, vTexCoord);
}

vec4 BoxBlur()
{
	//DO WHEN YOU GET HOME!
	vec2 texel = 1.0f / textureSize(target, 0).xy;

	// 9-tap box kernel
	vec4 colour = texture(target, vTexCoord);
	colour += texture(target, vTexCoord + vec2(-texel.x, texel.y));
	colour += texture(target, vTexCoord + vec2(-texel.x, 0));
	colour += texture(target, vTexCoord + vec2(-texel.x, -texel.y));
	colour += texture(target, vTexCoord + vec2(0, texel.y));
	colour += texture(target, vTexCoord + vec2(0, -texel.y));
	colour += texture(target, vTexCoord + vec2(texel.x, texel.y));
	colour += texture(target, vTexCoord + vec2(texel.x, 0));
	colour += texture(target, vTexCoord + vec2(texel.x, -texel.y));

	return colour / 9;
}

vec4 Distort()
{
	vec2 mid = vec2(0.5);

	float distanceFromCenter = distance(vTexCoord, mid);
	vec2 normalizedCoord = normalize(vTexCoord - mid);
	float bias = distanceFromCenter + sin(distanceFromCenter * 15) * 0.02;

	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(target, newCoord);
}

// Edge detection (coding-experiments.blogspot.com)
float threshold(in float thr1, in float thr2, in float val)
{
	if(val < thr1) {return 0.0;}
	if(val > thr2) {return 1.0;}
	return val;
}

// average pixel intensity from 3 colour channels
float avg_intensity(in vec4 pix)
{
	return (pix.r + pix.g + pix.b)/3;
}

vec4 get_pixel(in vec2 coords, in float dx, in float dy)
{
	return texture2D(target, coords + vec2(dx, dy));
}

// returns pixel colour
float IsEdge(in vec2 coords)
{

	float dxtex = 1.0 / 1280;
	float dytex = 1.0 / 1280;
	float pix[9];
	int k = -1;
	float delta;

	// read neighboring pixel intensities
	for(int i =-1; i < 2; i++)
	{
		for(int j =-1; j < 2; j++)
		{
			k++;
			pix[k] = avg_intensity(get_pixel(coords,
				 float(i)*dxtex, float(i)*dytex));
		}
	}

	// average colour difference around neighboring pixels
	delta = (abs(pix[1]-pix[7]) +
					 abs(pix[5]-pix[3]) +
					 abs(pix[0]-pix[8]) +
					 abs(pix[2]-pix[6]))/4;

	return threshold(0.25,0.4,clamp(1.8*delta,0.0,1.0));
}

void main()
{
	pixelColour = Distort();
}
