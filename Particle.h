#pragma once

#include <glm/vec3.hpp>

struct Particle
{
	Particle(glm::dvec3 position, double mass, bool fixed):
		position(position),
		previousPosition(position),
		velocity(0),
		normal(0,0,1),
		force(0),
		mass(mass),
		fixed(fixed)
	{}

	glm::dvec3 position;
	glm::dvec3 previousPosition;
	glm::dvec3 velocity;
	glm::dvec3 normal;
	glm::dvec3 force;

	double mass;

	bool fixed;

	void Move(double dt)
	{
		[[unlikely]]if (fixed) return;

		previousPosition = position;

		position = position * 2.0 - previousPosition + (force / mass) * (dt * dt);
		velocity = (position - previousPosition) / (2*dt);
	}
};