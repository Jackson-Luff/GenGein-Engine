#pragma once

#include <glm\glm.hpp>

struct CPUParticleVertex {
	glm::vec4 position;
	glm::vec4 colour;
};

struct ParticleData {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;
	float size;
	float lifetime;
	float lifespan;
};

struct GPUParticleVertex {
	GPUParticleVertex() :
	lifetime(1), lifespan(0) 
	{}

	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
	float lifespan;
};