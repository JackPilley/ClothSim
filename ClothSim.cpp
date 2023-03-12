#include <iostream>

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	SDL_GL_SetSwapInterval(1);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

	glViewport(0, 0, windowWidth, windowHeight);

	//We're going to use only one vertex array object because the scene is simple enough to just do everything manually
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	SDL_Event e;
	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
		}

		glClearColor(0.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

    return 0;
}
