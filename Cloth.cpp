#include "Cloth.h"
#include <iostream>
#include <algorithm>
#include <execution>
#include <glm/geometric.hpp>

Cloth::Cloth(double width, double height, GLuint xRes, GLuint yRes, double slack):
	xResolution(xRes),
	yResolution(yRes)
{
	//We need this to be true, and it can be false if certain preprocessor options are set
	static_assert(sizeof(glm::vec3) == sizeof(float) * 3);

	const GLuint vertexTotal = xResolution * yResolution;

	vertices.reserve(vertexTotal);
	particles.reserve(vertexTotal);

	const GLuint indexCount = (xResolution - 1) * (yResolution - 1) * 3 * 2;

	indices.reserve(indexCount);

	for (size_t y = 0; y < yResolution; ++y)
	{
		for (size_t x = 0; x < xResolution; ++x)
		{
			double xPos = width * static_cast<double>(x + 0.5) / static_cast<double>(xResolution) - width/2;
			double yPos = height * static_cast<double>(y + 0.5) / static_cast<double>(yResolution) - height/2;
			particles.emplace_back(glm::dvec3{ xPos, yPos, cos(xPos * 4)/4 }, 1, false);

			vertices.emplace_back(glm::vec3{0.f}, glm::vec3{0.f, 0.f, 1.f});
		}
	}

	GLuint curX = 0;
	GLuint curY = 0;

	for (size_t i = 0; i < indexCount / 6; i++)
	{
		indices.push_back(curX + curY * xRes);
		indices.push_back(curX + (curY + 1) * xRes);
		indices.push_back(curX + (curY + 1) * xRes + 1);

		indices.push_back(curX + curY * xRes);
		indices.push_back(curX + (curY + 1) * xRes + 1);
		indices.push_back(curX + curY * xRes + 1);

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

	for (size_t i = 0; i < indices.size(); i += 3)
	{
		faceParRange.push_back(i);
		normsIntermediate.push_back(glm::vec3{ 0 });
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices.front(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
}

void Cloth::UpdateGeometry()
{
	size_t i = 0;
	//Update positions
	for (auto& particle : particles)
	{
		vertices[i++].pos = glm::vec3{ particle.position };
	}

	//Reset normals
	for (auto& norm : normsIntermediate)
	{
		norm = glm::vec3{ 0 };
	}

	// Recalc normals in parallel
	std::for_each(std::execution::par, faceParRange.begin(), faceParRange.end(), [this](size_t& i)
		{
			size_t indA = indices[i];
			size_t indB = indices[i + 1];
			size_t indC = indices[i + 2];

			glm::vec3& a = vertices[indA].pos;
			glm::vec3& b = vertices[indB].pos;
			glm::vec3& c = vertices[indC].pos;


			normsIntermediate[i/3] += glm::cross(c - a, b - a);
		});

	//Normalize normals in parallel
	std::for_each(std::execution::par, normsIntermediate.begin(), normsIntermediate.end(), [](glm::vec3& norm)
		{
			norm = glm::normalize(norm);
		});

	//Apply normals to vertices
	//Note there's a lot of redundant assignments happening here, I'm not sure how to rework
	//the code to get rid of them.
	i = 0;
	for (auto& norm : normsIntermediate)
	{
		size_t indA = indices[i];
		size_t indB = indices[i + 1];
		size_t indC = indices[i + 2];

		vertices[indA].norm = norm;
		vertices[indB].norm = norm;
		vertices[indC].norm = norm;

		i += 3;
	}

	for (i = 0; i < vertices.size(); i++)
	{
		particles[i].normal = vertices[i].norm;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices.front());
}

void Cloth::Step(double dt)
{
	for (auto& spring : springs)
	{
		spring.CalcTension();
	}

	for (auto& particle : particles)
	{
		particle.Move(dt);
	}
}

void Cloth::SetParticlePosition(size_t x, size_t y, glm::dvec3 position)
{

}

void Cloth::SetParticleFixed(size_t x, size_t y, bool fixed)
{

}

void Cloth::Draw()
{
	UpdateGeometry();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}