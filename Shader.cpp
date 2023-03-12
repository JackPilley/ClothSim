#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{
	std::string vertSource, fragSource;

	std::ifstream vertShaderFile, fragShaderFile;

	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertShaderFile.open(vertPath);
		fragShaderFile.open(fragPath);

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
	char infoLog[1024];

	// lvalue needed for glShaderSource
	const char* vertSourceL = vertSource.c_str();

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSourceL, NULL);
	glCompileShader(GL_VERTEX_SHADER);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error compiling vertex shader: " << infoLog << "\n";
	}

	// lvalue needed for glShaderSource
	const char* fragSourceL = vertSource.c_str();

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert, 1, &fragSourceL, NULL);
	glCompileShader(GL_FRAGMENT_SHADER);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error compiling fragment shader: " << infoLog << "\n";
	}

	ID = glCreateProgram();
	glAttachShader(ID, vert);
	glAttachShader(ID, frag);

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error linking shader program: " << infoLog << "\n";
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Use()
{
	glUseProgram(ID);
}