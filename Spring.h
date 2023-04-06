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

	double GetDeformationRate()
	{
		double length = glm::distance(A.position, B.position);
		return (length - restLength) / restLength;
	}

	void ResolveSuperElasticity()
	{
		if (A.fixed && B.fixed) return;

		double deformationRate = GetDeformationRate();

		if (deformationRate > 0.0)
		{
			if (A.fixed || B.fixed)
			{
				Particle& fixed = A.fixed ? A : B;
				Particle& unfixed = A.fixed ? B : A;

				glm::dvec3 dir = glm::normalize(unfixed.position - fixed.position);
				unfixed.position = fixed.position + dir * restLength;
			}
			else
			{
				glm::dvec3 dir = glm::normalize(A.position - B.position);
				glm::dvec3 mid = (A.position + B.position) / 2.0;
				A.position = mid + dir * restLength / 2.0;
				B.position = mid - dir * restLength / 2.0;

			}
		}
	}
};