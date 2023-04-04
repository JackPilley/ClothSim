#pragma once

#include "Particle.h"
#include <glm/geometric.hpp>

struct Spring
{
	Spring(Particle& A, Particle& B, double restLength, double stiffness, double damping):
		A(A),
		B(B),
		restLength(restLength),
		stiffness(stiffness),
		damping(damping),
		force(0)
	{}

	Particle& A;
	Particle& B;

	double restLength;
	double stiffness;
	double damping;

	glm::dvec3 force;

	void CalcTension()
	{
		glm::dvec3 dir = glm::normalize(A.position - B.position);

		//Spring force
		force = stiffness * (restLength - glm::length(A.position - B.position)) * dir;

		//calculate spring damper force
		force += -damping * glm::dot(A.velocity - B.velocity, dir) * dir;
	}

	void ApplyForce()
	{
		A.force += force;
		B.force += -force;
	}
};