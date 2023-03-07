#pragma once

#include <glm/vec3.hpp>

struct Particle
{
	glm::dvec3 position;
	glm::dvec3 velocity;
	glm::dvec3 force;

	double mass;

	bool fixed;

	void Move(double dt);
};