#version 410

in vec2 vTexCoord;
out vec4 pixelColour;

uniform sampler2D ScreenPrintTexture;
uniform float time;

// AIE Interpretations
vec4 Simple(in sampler2D textureSample, in vec2 uvCoords)
{
	return texture(textureSample, uvCoords);
}

vec4 BoxBlur(in sampler2D textureSample, in vec2 uvCoords)
{
	vec2 texel = 1.0f / textureSize(textureSample, 0).xy;

	// 9-tap box kernel
	vec4 colour = texture(textureSample, vTexCoord);
	colour += texture(textureSample, uvCoords + vec2(-texel.x, texel.y));
	colour += texture(textureSample, uvCoords + vec2(-texel.x, 0));
	colour += texture(textureSample, uvCoords + vec2(-texel.x, -texel.y));
	colour += texture(textureSample, uvCoords + vec2(0, texel.y));
	colour += texture(textureSample, uvCoords + vec2(0, -texel.y));
	colour += texture(textureSample, uvCoords + vec2(texel.x, texel.y));
	colour += texture(textureSample, uvCoords + vec2(texel.x, 0));
	colour += texture(textureSample, uvCoords + vec2(texel.x, -texel.y));

	return colour / 9;
}

vec4 Distort(in sampler2D textureSample, in vec2 uvCoords)
{
	vec2 mid = vec2(0.5);

	float distanceFromCenter = distance(uvCoords, mid);
	vec2 normalizedCoord = normalize(uvCoords - mid);
	float bias = distanceFromCenter + sin(distanceFromCenter * 15) * 0.02;

	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(textureSample, newCoord);
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

vec4 get_pixel(in sampler2D textureSample, in vec2 uvCoords, in float dx, in float dy)
{
	return texture2D(textureSample, uvCoords + vec2(dx, dy));
}

// returns pixel colour
float IsEdge(in sampler2D sampleTexture, in vec2 uvCoords)
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
			pix[k] = avg_intensity(get_pixel(sampleTexture, uvCoords,
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
	pixelColour = vec4(1);//Simple(ScreenPrintTexture, vTexCoord);
}
