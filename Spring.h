#pragma once

#include "Particle.h"

struct Spring
{
	Particle& A;
	Particle& B;

	double restLength;
	double stiffness;
	double damping;

	void ApplyForces();
};