#pragma once

#include "Particle.h"
#include <glm/geometric.hpp>

#define MAX_DEFORM 0.05

struct Spring
{
	Spring(Particle& A, Particle& B, double restLength, double stiffness):
		A(&A),
		B(&B),
		restLength(restLength),
		stiffness(stiffness),
		force(0),
		deformationRate(0)
	{}

	Particle* A;
	Particle* B;

	double restLength;
	double stiffness;

	glm::dvec3 force;

	double deformationRate;

	void CalcTension()
	{
		glm::dvec3 dir = glm::normalize(A->position - B->position);

		//Spring force
		//force = stiffness * (restLength - glm::length(A->position - B->position)) * dir;
		force = stiffness * ((A->position - B->position) - restLength * dir);
	}

	void CalcDeformationRate()
	{
		deformationRate = (glm::distance(A->position, B->position) - restLength) / restLength;
	}

	void ApplyForce()
	{
		A->force += -force;
		B->force += force;
	}

	void ResolveSuperElongation()
	{
		CalcDeformationRate();
		[[likely]] if (deformationRate < MAX_DEFORM) return;
		if ((A->fixed) && (B->fixed)) return;

		if (A->fixed || B->fixed)
		{
			Particle& fixed = A->fixed ? *A : *B;
			Particle& unfixed = A->fixed ? *B : *A;

			glm::dvec3 dir = glm::normalize(unfixed.position - fixed.position);
			unfixed.position = fixed.position + dir * (restLength * (1.0 + MAX_DEFORM));
		}
		else
		{
			glm::dvec3 midpoint = (A->position + B->position) / 2.0;
			glm::dvec3 toA = glm::normalize(A->position - midpoint);
			glm::dvec3 toB = glm::normalize(B->position - midpoint);
			A->position = midpoint + toA * (restLength * (1.0 + MAX_DEFORM) /2.0);
			B->position = midpoint + toB * (restLength * (1.0 + MAX_DEFORM) /2.0);
		}
	}
};