#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Shader::Shader()
{
	std::string vertSource, fragSource;

	std::ifstream vertShaderFile, fragShaderFile;

	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertShaderFile.open("vert.glsl");
		fragShaderFile.open("frag.glsl");

		std::stringstream vertStream, fragStream;

		vertStream << vertShaderFile.rdbuf();
		fragStream << fragShaderFile.rdbuf();

		vertShaderFile.close();
		fragShaderFile.close();

		vertSource = vertStream.str();
		fragSource = fragStream.str();
	}
	catch (std::ifstream::failure ex)
	{
		std::cerr << "Error reading shader files: " << ex.what() << "\n";
	}

	GLuint vert, frag;
	GLint success;
	char infoLog[512];

	// lvalue needed for glShaderSource
	const char* vertSourceL = vertSource.c_str();

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSourceL, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error compiling vertex shader: " << std::string{ infoLog } << "\n";
	}

	// lvalue needed for glShaderSource
	const char* fragSourceL = fragSource.c_str();

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSourceL, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error compiling fragment shader: " << std::string{ infoLog } << "\n";
	}

	ID = glCreateProgram();
	glAttachShader(ID, vert);
	glAttachShader(ID, frag);

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error linking shader program: " << std::string{ infoLog } << "\n";
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	glUseProgram(ID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	mvLoc = glGetUniformLocation(ID, "uModelViewMatrix");
	normLoc = glGetUniformLocation(ID, "uNormalMatrix");
	projLoc = glGetUniformLocation(ID, "uProjectionMatrix");
	viewLoc = glGetUniformLocation(ID, "uViewMatrix");
	lightLoc = glGetUniformLocation(ID, "uLightDirection");
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetMVMatrix(const glm::mat4& model, const glm::mat4& view)
{
	glm::mat4 mv = view * model;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat3 norm = glm::inverseTranspose(glm::mat3{mv});

	glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(norm));
}

void Shader::SetProjMatrix(const glm::mat4& proj)
{
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}

void Shader::SetLightVector(const glm::vec3& light)
{
	glUniform3fv(lightLoc, 1, glm::value_ptr(glm::normalize(light)));
}