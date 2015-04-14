#version 410

in layout(location=0) vec3 Position;
in layout(location=1) vec3 Velocity;
in layout(location=2) float Lifetime;
in layout(location=3) float Lifespan;

out vec3 vPosition;
out vec3 vVelocity;
out float vLifetime;
out float vLifespan;

uniform vec3 SpherePos;
uniform vec3 emitterPosition;
uniform float time;
uniform float deltaTime;
uniform float lifeMin;
uniform float lifeMax;
uniform float veloMin;
uniform float veloMax;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

float waterLevel = -2.5;

vec3 newVel;

void main()
{
	
	float dt = deltaTime + deltaTime;
	newVel = Velocity;
		
	float a = Lifetime, b = Lifespan, c = 0.5, d = 0.1;
	newVel.x = (a-b)*sin(time) + b * sin(time * ((a/b)-1));
	newVel.y = (a-b)*sin(time) + b * cos(time * ((a/b)-1));
	newVel.z = (a-b)*cos(time) - b * cos(time * ((a/b)-1));

	vec3 FirstTarget = vec3(sin(time), 0, cos(time));
	vec3 SecondTarget = vec3(cos(time),0, sin(time));
	vec3 dir;

	if((int(time) % 2) == 0)
		dir = normalize(FirstTarget - Position);
	else 
		dir = normalize(SecondTarget - Position);
		
	newVel += dir;
	newVel.x += sin(dir.x) / 3.1415695 ;
	newVel.z += cos(dir.z) / 3.1415695 ;

	vPosition = Position + (Velocity * dt);
	vVelocity = newVel;
	vLifetime = Lifetime + dt;
	vLifespan = Lifespan;
	
	float sphereRadius = 1;
	float particleRadius = 0.05;
	float dist = length(Position);

	/* vPosition.x += dir.x * newVel.x; */

	// emit a new particle as soon as it dies
	if(vLifetime > vLifespan)
	{
		uint seed = uint(time * 1000.0) + uint(gl_VertexID);
		//Initialise velocities
		vVelocity.x = rand(seed++, 2) - 1 + (0.2 * cos(time));
		vVelocity.y = rand(seed++, 2) - 1 * time;
		vVelocity.z = rand(seed++, 2) - 1 + (0.2 * sin(time));
		vVelocity = normalize(vVelocity);
		vPosition = emitterPosition;
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
