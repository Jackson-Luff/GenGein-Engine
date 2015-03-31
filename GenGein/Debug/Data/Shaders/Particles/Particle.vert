#version 410

in layout(location=0) vec3 Position;
in layout(location=1) vec3 Velocity;
in layout(location=2) float Lifetime;
in layout(location=3) float Lifespan;

out vec3 vPosition;
out vec2 vCoords;
out float vLifetime;
out float vLifespan;

void main() 
{
	vPosition = Position;
	vLifetime = Lifetime;
    vLifespan = Lifespan;
}