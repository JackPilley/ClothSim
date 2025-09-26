#pragma once

#include "Particle.h"
#include "Spring.h"
#include <vector>
#include <glad/glad.h>
#include "Vertex.h"

//#define NO_STORED_SPRINGS

class Cloth
{
	GLuint xResolution;
	GLuint yResolution;

	std::vector<Particle> particles;

#ifndef NO_STORED_SPRINGS
	std::vector<Spring> structuralSprings;
	std::vector<Spring> shearSprings;
	std::vector<Spring> flexionSprings;
#endif

	const double structuralStiffness = 35.0;
	const double shearStiffness = 35.0;
	const double flexionStiffness = 5.0;

	double horizontalStructuralRestLength;
	double verticalStructuralRestLength;
	double shearRestLength;
	double horizontalFlexionRestLength;
	double verticalFlexionRestLength;

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
	void ResetForces();
	void CalcTensions();
	void ApplyForces();
	void ApplyWorldForces();
	void ResolveSuperElongations();

	bool resolveSuperElongation = true;

public:
	Cloth() = delete;
	Cloth(double width, double height, GLuint xRes, GLuint yRes);
	void Step(double dt);
	void SetParticlePosition(size_t x, size_t y, glm::dvec3 position);
	void SetParticleFixed(size_t x, size_t y, bool fixed);

	void ToggleElongationResolution() { resolveSuperElongation = !resolveSuperElongation; };

	void Draw();
};