#pragma once

#include <glm/vec3.hpp>

struct Vertex
{
	Vertex() = default;
	Vertex(glm::vec3 pos, glm::vec3 norm):
		pos(pos), norm(norm){}

	glm::vec3 pos;
	glm::vec3 norm;
};