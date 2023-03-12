#pragma once

#include <glm/vec3.hpp>

struct Particle
{
	Particle(glm::dvec3 position, double mass, bool fixed):
		position(position),
		velocity(0),
		force(0),
		mass(mass),
		fixed(fixed)
	{}

	glm::dvec3 position;
	glm::dvec3 velocity;
	glm::dvec3 force;

	double mass;

	bool fixed;

	void Move(double dt);
};