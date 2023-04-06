#pragma once

#include "Particle.h"
#include "Spring.h"
#include <vector>
#include <glad/glad.h>
#include "Vertex.h"

class Cloth
{
	GLuint xResolution;
	GLuint yResolution;

	std::vector<Particle> particles;

	std::vector<Spring> springs;
	std::vector<Spring*> structuralSprings;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	//Vertex buffer object
	GLuint vbo;
	GLuint ebo;

	//Indices used for parallel normal calculation
	std::vector<size_t> faceParRange;
	//Intermediate storage for normal generation
	std::vector<glm::vec3> normsIntermediate;
	
	void UpdateGeometry();

public:
	Cloth() = delete;
	Cloth(double width, double height, GLuint xRes, GLuint yRes, double slack);
	void Step(double dt);
	void SetParticlePosition(size_t x, size_t y, glm::dvec3 position);
	void SetParticleFixed(size_t x, size_t y, bool fixed);

	void Draw();
};