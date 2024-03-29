#include <iostream>
#include <chrono>

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Cloth.h"

int main(int argc, char* argv[])
{
	const int windowWidth = 800;
	const int windowHeight = 800;

	SDL_Window* window;
	SDL_GLContext glContext;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error starting SDL: " << SDL_GetError() << "\n";
		return 1;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cerr << "Error starting IMG: " << IMG_GetError() << "\n";
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	

	window = SDL_CreateWindow("Cloth Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cerr << "Error creating window: " << SDL_GetError() << "\n";
		return 1;
	}

	glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
		std::cerr << "Error creating gl context: " << SDL_GetError() << "\n";
		return 1;
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cerr << "Error initializing glad\n";
		return 1;
	}

	SDL_GL_SetSwapInterval(0);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int clothRes = 20;

	Cloth cloth(5.0, 5.0, clothRes, clothRes, 1);
	//for (int i = 0; i < clothRes; i++)
	//{
	//	cloth.SetParticleFixed(0, i, true);
	//}
	cloth.SetParticleFixed(0, clothRes-1, true);
	//cloth.SetParticleFixed(0, clothRes/2, true);
	cloth.SetParticleFixed(0, 0, true);

	Shader shader{};
	shader.Use();
	
	shader.SetProjMatrix(glm::perspective(glm::radians(80.f), 1.f, 0.1f, 100.f));
	shader.SetLightVector(glm::vec3(-1, -1, -1));
	glm::mat4 transform{ 1.f };
	transform = glm::translate(transform, glm::vec3{ 0.f, 0.f, -1.f });
	glm::mat4 view{ 1.f };
	view = glm::lookAt(glm::vec3{ 0,0.5,1 }, glm::vec3{ 0,0,-1.f }, glm::vec3{0,1,0});
	//transform = glm::rotate(transform, glm::pi<float>() / 1, glm::vec3{ 0,1,0 });
	shader.SetMVMatrix(transform, view);

	using namespace std::chrono;

	SDL_Event e;
	bool quit = false;
	double time = 0.0167;
	while (!quit)
	{
		high_resolution_clock::time_point start = high_resolution_clock::now();
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
					cloth.ToggleElongationResolution();
			}
		}

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		cloth.Step(time);
		cloth.Draw();

		high_resolution_clock::time_point end = high_resolution_clock::now();

		SDL_GL_SwapWindow(window);

		duration<double> timeSpan = duration_cast<duration<double>>(end - start);
		std::cout << timeSpan.count() * 1000 << "\n";
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

    return 0;
}
