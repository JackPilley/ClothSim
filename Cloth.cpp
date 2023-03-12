#include "Cloth.h"

Cloth::Cloth(double width, double height, size_t xRes, size_t yRes, double slack):
	xResolution(xRes),
	yResolution(yRes)
{
	//We need this to be true, and it can be false if certain preprocessor options are set
	static_assert(sizeof(glm::vec3) == sizeof(float) * 3);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	vertices.reserve(xResolution * yResolution);
	particles.reserve(xResolution * yResolution);

	for (size_t y = 0; y < yResolution; ++y)
	{
		for (size_t x = 0; x < xResolution; ++x)
		{
			double xPos = width * static_cast<double>(x) / static_cast<double>(xResolution) - width/2;
			double yPos = height * static_cast<double>(y) / static_cast<double>(yResolution) - height/2;
			particles.emplace_back(glm::dvec3{ xPos, yPos, 0.0 }, 1, false);

			vertices.emplace_back(xPos, yPos, 0.f);
		}
	}

	glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices.front(), GL_DYNAMIC_DRAW);
}

void Cloth::Step(double dt)
{

}

void Cloth::SetParticlePosition(size_t x, size_t y, glm::dvec3 position)
{

}

void Cloth::SetParticleFixed(size_t x, size_t y, bool fixed)
{

}

void Cloth::Draw()
{

}