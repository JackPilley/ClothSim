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

		glm::dvec3 accel = force / mass;
		velocity = velocity + accel * dt;
		position = position + velocity * dt;
	}

	void CalcActualVelocity(double dt)
	{
		velocity = (position - previousPosition) / dt;
	}
};