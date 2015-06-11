#version 410

in layout(location=0) vec3 Position;
in layout(location=1) vec3 Velocity;
in layout(location=2) float Lifetime;
in layout(location=3) float Lifespan;

out vec3 vPosition;
out vec3 vVelocity;
out float vLifetime;
out float vLifespan;

uniform mat4 World;
uniform vec3 emitterPosition;
uniform float time;
uniform float deltaTime;
uniform float lifeMin;
uniform float lifeMax;
uniform float veloMin;
uniform float veloMax;
uniform float isKeyDown;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

vec3 newVel;

float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

void main()
{
	uint seed = uint(time * 1000.0) + uint(gl_VertexID);
	float dt = deltaTime;
		
	newVel = Velocity;
	
	vec3 Target = vec3(sin(time), cos(time), tan(time));
	vec3 dir = normalize(Target - Position);
	newVel += dir;
	
	//float a = Lifetime, b = Lifespan, c = 0.5, d = 0.5;
	//newVel.x += (a-b)*sin(time) + b * sin(time * ((a/b)-1));
	//newVel.y += (a-b)*sin(time) - b * sin(time * ((a/b)-1));
	//newVel.z += (a-b)*cos(time) - b * cos(time * ((a/b)-1));
	
	//newVel.y = sin(newVel.y) + ( Lifetime/Lifespan * cos(time));
	
	vPosition = Position + (Velocity * dt);
	vVelocity = newVel;
	vLifetime = Lifetime + dt;
	vLifespan = Lifespan;
	
	vPosition = clamp(vPosition, -1,1);
	// if it's dead spawn a new one!
	if(vLifetime > vLifespan)
	{			
		//Initialise velocity
		vVelocity.x = rand(seed++, 2) - 1;
		vVelocity.y = rand(seed++, 2) - 1;
		vVelocity.z = rand(seed++, 2) - 1;
		vVelocity = normalize(vVelocity);
		vPosition = emitterPosition + vec3(sin(time)*0.5, tan(time)*0.5, cos(time)*0.5);
		vLifetime = 0;
		vLifespan = rand(seed++, lifeMax - lifeMin) + lifeMin;
	}
}

/*
//Weird worm slinky
if((int(Position.x) % 2) == 0)
	newVel.x += (0.5 * sin(pow(time, 2)));
if((int(Position.x) % 2) == 0)
	newVel.y += (0.2 * clamp(tan(time), -10, 10));
if((int(Position.x) % 2) == 0)
	newVel.z += (0.5 * cos(pow(time, 2)));

	====================================

//Cube slap effect
if((int(Position.x) % 2) == 0)
	newVel.x += (0.5 * sin(time));
if((int(Position.y) % 2) == 0)
	newVel.y += (0.2 * clamp(tan(time), -10, 10));
if((int(Position.z) % 2) == 0)
	newVel.z += (0.5 * cos(time));

	====================================

// AMAZING Spiral fan!
	float a = Lifetime, b = Lifespan, c = 0.5, d = 0.5;
	newVel.x = (a-b)*cos(time) + b * cos(time * ((a/b)-1));
	newVel.z = (a-b)*sin(time) - b * sin(time * ((a/b)-1));

	=====================================

// Playstation background theme like effect? (web)
	vec3 Target = vec3(sin(time),cos(time),tan(time));
	vec3 Dir = normalize(Target - Position);
	newVel += Dir;

// WEIRD optical illusion
	vVelocity.x = rand(seed++, 2) / veloMax + sin(time);
	vVelocity.y = rand(seed++, 2) / veloMax + cos(time);
	vVelocity.z = rand(seed++, 2) / veloMax + tan(time);

// BLACK HOLE!
	float a = Lifetime, b = Lifespan, c = 0.5, d = 0.5;
	newVel.x = (a-b)*sin(time) + b * sin(time * ((a/b)-1));
	newVel.y = (a-b)*sin(time) / b * cos(time * ((a/b)-1));
	newVel.z = (a-b)*cos(time) - b * cos(time * ((a/b)-1));
	//Initialise velocities
	vVelocity.x = rand(seed++, 2) / veloMax + sin(time);
	vVelocity.y = rand(seed++, 2) / veloMax + cos(time);
	vVelocity.z = rand(seed++, 2) / veloMax + tan(time);

//DeltaTime check?
	vec3 Target = vec3(0, -0.5f, 0);
	vec3 Dir = normalize(Target - Position);
	newVel += Dir;
	//Initialise velocities
	vVelocity.x = rand(seed++, 2) - 1 + (0.2 * cos(time));
	vVelocity.y = rand(seed++, 2) - 1 * time;
	vVelocity.z = rand(seed++, 2) - 1 + (0.2 * sin(time));
*/
