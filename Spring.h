#pragma once

#include "Particle.h"
#include <glm/geometric.hpp>

struct Spring
{
	Spring(Particle& A, Particle& B, double restLength, double stiffness):
		A(A),
		B(B),
		restLength(restLength),
		stiffness(stiffness),
		force(0)
	{}

	Particle& A;
	Particle& B;

	double restLength;
	double stiffness;

	glm::dvec3 force;

	void CalcTension()
	{
		glm::dvec3 dir = glm::normalize(A.position - B.position);

		//Spring force
		force = stiffness * (restLength - glm::length(A.position - B.position)) * dir;
	}

	void ApplyForce()
	{
		A.force += force;
		B.force += -force;
	}
};