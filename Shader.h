#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

class Shader
{
private:
	GLuint ID;
	GLint mvLoc;
	GLint normLoc;
	GLint projLoc;
	GLint viewLoc;
	GLint lightLoc;
public:
	//This function can fail, but doesn't provide any status we can check, so look in the console if things are behaving weird
	Shader();
	~Shader();
	void Use();
	void SetMVMatrix(const glm::mat4& model, const glm::mat4& view);
	void SetProjMatrix(const glm::mat4& proj);
	void SetLightVector(const glm::vec3& light);
};