#pragma once

#include "Particle.h"
#include "Spring.h"
#include <vector>

class Cloth
{
	size_t xResolution;
	size_t yResolution;

	std::vector<Particle> particles;

	std::vector<Spring> structuralSprings;
	std::vector<Spring> shearSprings;
	std::vector<Spring> flexionSprings;

public:
	Cloth(double width, double height, size_t xRes, size_t yRes, double slack);
	void Step(double dt);
	void SetParticlePosition(size_t x, size_t y, glm::dvec3 position);
	void SetParticleFixed(size_t x, size_t y, bool fixed);

	void Draw();
};