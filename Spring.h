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

	const double restLength;
	const double stiffness;

	glm::dvec3 force;

	double deformationRate;

	void CalcTension()
	{
		glm::dvec3 dir = glm::normalize(A->position - B->position);

		//Spring force
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

	static void CalcAndApplyForce(Particle& a, Particle& b, double restLength, double stiffness)
	{
		glm::dvec3 dir = glm::normalize(a.position - b.position);
		glm::dvec3 force = stiffness * ((a.position - b.position) - restLength * dir);
		a.force += -force;
		b.force += force;
	}

	static void ResolveSuperElongation(Particle& a, Particle& b, double restLength)
	{
		double deformationRate = (glm::distance(a.position, b.position) - restLength) / restLength;
		[[likely]] if (deformationRate < MAX_DEFORM) return;
		if ((a.fixed) && (b.fixed)) return;

		if (a.fixed || b.fixed)
		{
			Particle& fixed = a.fixed ? a : b;
			Particle& unfixed = a.fixed ? b : a;

			glm::dvec3 dir = glm::normalize(unfixed.position - fixed.position);
			unfixed.position = fixed.position + dir * (restLength * (1.0 + MAX_DEFORM));
		}
		else
		{
			glm::dvec3 midpoint = (a.position + b.position) / 2.0;
			glm::dvec3 toA = glm::normalize(a.position - midpoint);
			glm::dvec3 toB = glm::normalize(b.position - midpoint);
			a.position = midpoint + toA * (restLength * (1.0 + MAX_DEFORM) / 2.0);
			b.position = midpoint + toB * (restLength * (1.0 + MAX_DEFORM) / 2.0);
		}
	}
};