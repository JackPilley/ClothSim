#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
private:
	GLuint ID;
public:
	//This function can fail, but doesn't provide any status we can check, so look in the console if things are behaving weird
	Shader();
	~Shader();
	void Use();
};