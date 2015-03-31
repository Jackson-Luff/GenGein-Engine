#version 430

in vec4 vPosition;
in vec2 vCoords;

uniform mat4 World;
uniform float time;

vec4 mod289(vec4 x)
{
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t)
{
	return t * t * t * ( t * ( t * 6.0 - 15.0 ) + 10.0 );
}

float cnoise(vec2 P, vec2 rep)
{
	vec4 Pi = floor(P.xyxy) + vec4(0.0,0.0,1.0,1.0);
	vec4 Pf = fract(P.xyxy) - vec4(0.0,0.0,1.0,1.0);
	Pi = mod(Pi, rep.xyxy);  // To create noise with explicit period
	Pi = mod289(Pi);		 // To avoid truncation effects in permutation
	vec4 ix = Pi.xzxz;
	vec4 iy = Pi.yyww;
	vec4 fx = Pf.xzxz;
	vec4 fy = Pf.yyww;

	vec4 i = permute( permute(ix) + iy );

	vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0;
	vec4 gy = abs(gx) - 0.5;
	vec4 tx = floor(gx + 0.5);
	gx = gx - tx;

	vec2 g00 = vec2(gx.x, gy.x);
	vec2 g10 = vec2(gx.y, gy.y);
	vec2 g01 = vec2(gx.z, gy.z);
	vec2 g11 = vec2(gx.w, gy.w);

	vec4 norm = taylorInvSqrt( vec4(dot(g00,g00), dot(g01,g01), dot(g10,g10), dot(g11,g11) ) );
	g00 *= norm.x;
	g01 *= norm.y;
	g10 *= norm.z;
	g11 *= norm.w;

	float n00 = dot(g00, vec2(fx.x, fy.x));
	float n10 = dot(g10, vec2(fx.y, fy.y));
	float n01 = dot(g01, vec2(fx.z, fy.z));
	float n11 = dot(g11, vec2(fx.w, fy.w));

	vec2 fade_xy = fade(Pf.xy);
	vec2 n_x = mix(vec2(n00,n01), vec2(n10, n11), fade_xy.x);
	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
	return 2.3 * n_xy;
}

float cnoise(vec2 P)
{
	vec4 Pi = floor(P.xyxy) + vec4(0.0,0.0,1.0,1.0);
	vec4 Pf = fract(P.xyxy) - vec4(0.0,0.0,1.0,1.0);
	Pi = mod289(Pi);		 // To avoid truncation effects in permutation
	vec4 ix = Pi.xzxz;
	vec4 iy = Pi.yyww;
	vec4 fx = Pf.xzxz;
	vec4 fy = Pf.yyww;

	vec4 i = permute( permute(ix) + iy );

	vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0;
	vec4 gy = abs(gx) - 0.5;
	vec4 tx = floor(gx + 0.5);
	gx = gx - tx;

	vec2 g00 = vec2(gx.x, gy.x);
	vec2 g10 = vec2(gx.y, gy.y);
	vec2 g01 = vec2(gx.z, gy.z);
	vec2 g11 = vec2(gx.w, gy.w);

	vec4 norm = taylorInvSqrt( vec4(dot(g00,g00), dot(g01,g01), dot(g10,g10), dot(g11,g11) ) );
	g00 *= norm.x;
	g01 *= norm.y;
	g10 *= norm.z;
	g11 *= norm.w;

	float n00 = dot(g00, vec2(fx.x, fy.x));
	float n10 = dot(g10, vec2(fx.y, fy.y));
	float n01 = dot(g01, vec2(fx.z, fy.z));
	float n11 = dot(g11, vec2(fx.w, fy.w));

	vec2 fade_xy = fade(Pf.xy);
	vec2 n_x = mix(vec2(n00,n01), vec2(n10, n11), fade_xy.x);
	float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
	return 2.3 * n_xy;
}

float fbm(vec2 vPos, float per, int oc)
{
	int oc0 = 0;
	float val = 0;

	for(oc0 = 0; oc0 < oc; oc0+=1)
	{
		val += pow(0.5,oc0) * cnoise( vPos * pow(2.0, oc0), vPos * per*pow(2.0,oc0));
	}

	return val;
}


float fbm(vec2 P, int octaves, float lacunarity, float gain)
{
	float sum = 0.0;
	float amp = 1.0;
	vec2 pp = P;
	int i;
	for(i = 0; i < octaves; i+=1)
	{
		amp *= gain;
		sum += amp * cnoise(pp);
		pp *= lacunarity;
	}
	return sum;
}

vec2 Distort(vec2 vCoords)
{
	vec2 mid = vec2(0.5);

	float distanceFromCenter = distance(vCoords, mid);
	vec2 normalizedCoord = normalize(vCoords - mid);
	float bias = distanceFromCenter + sin(distanceFromCenter * 15) * 0.02;

	vec2 newCoord = mid + bias * normalizedCoord;
	return newCoord;
}

float pattern(in vec2 p)
{
	float l = 2.5;
	float g = 0.4;
	int oc = 10;

	vec2 q = vec2( fbm( p + vec2(0.0,0.0),oc,l,g),fbm( p + vec2(5.2,1.3),oc,l,g));
  vec2 r = vec2( fbm( p + 4.0*q + vec2(1.7,9.2),oc,l,g ), fbm( p + 4.0*q + vec2(8.3,2.8) ,oc,l,g));
  return fbm( p + 4.0*r ,oc,l,g);
}

float pattern2( in vec2 p, out vec2 q, out vec2 r , in float time)
{
    float l = 2.3;
    float g = 0.4;
    int oc = 10;

    q.x = fbm( p + vec2(time,time),oc,l,g);
    q.y = fbm( p + vec2(5.2*time,1.3*time) ,oc,l,g);

    r.x = fbm( p + 4.0*q + vec2(1.7,9.2),oc,l,g );
    r.y = fbm( p + 4.0*q + vec2(8.3,2.8) ,oc,l,g);

	return fbm( (p + 4.0 * r) , oc, l, g);
}

void main()
{
	//perlin data
	vec2 q = (vPosition.xz+World[3].xz);
//	q += dir.xy;
	vec2 p = -1.0 + 2.0 * q;
	vec2 qq;
	vec2 r;
	float perlin = pattern2(p,qq,r,time*0.05);

	vec4 perl = vec4(perlin, perlin, perlin, perlin);
	//perl *= 30.5;

	// colouring
	vec4 darkBlue = vec4(0,0,1,1);
	vec4 aqua 		= vec4(0,1,1,1);

	vec4 finalColour = (aqua * perl.a);

	finalColour = finalColour + (1 - perl.a) * darkBlue;
	finalColour.a = 0.5;
	gl_FragColor = finalColour;
}
