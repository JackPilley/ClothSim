#include "Cloth.h"

Cloth::Cloth(double width, double height, size_t xRes, size_t yRes, double slack):
	xResolution(xRes),
	yResolution(yRes)
{
	//We need this to be true, and it can be false if certain preprocessor options are set
	static_assert(sizeof(glm::vec3) == sizeof(float) * 3);
	
	vertices.reserve(xResolution * yResolution);
	particles.reserve(xResolution * yResolution);

	const size_t indexCount = (xResolution - 1) * (yResolution - 1) * 3 * 2;

	indices.reserve(indexCount);

	for (size_t y = 0; y < yResolution; ++y)
	{
		for (size_t x = 0; x < xResolution; ++x)
		{
			double xPos = width * static_cast<double>(x) / static_cast<double>(xResolution) - width/2;
			double yPos = height * static_cast<double>(y) / static_cast<double>(yResolution) - height/2;
			particles.emplace_back(glm::dvec3{ xPos, yPos, 0.0 }, 1, false);

			vertices.emplace_back(glm::vec3{static_cast<float>(xPos), static_cast<float>(yPos), 0.f}, glm::vec3{0.f, 0.f, 1.f});
		}
	}

	size_t curX = 0;
	size_t curY = 0;

	for (size_t i = 0; i < indexCount / 6; i++)
	{
		indices.push_back(curX + curY * xRes);
		indices.push_back(curX + (curY + 1) * xRes);
		indices.push_back(curX + (curY + 1) * xRes + 1);

		indices.push_back(curX + curY * xRes);
		indices.push_back(curX + curY * xRes + 1);
		indices.push_back(curX + (curY + 1) * xRes + 1);

		if (curX == xResolution - 2)
		{
			curX = 0;
			curY += 1;
		}
		else
		{
			curX += 1;
		}
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices.front(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
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