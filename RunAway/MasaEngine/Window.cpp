#include "Window.h"
#include "MasaEngineError.h"

namespace MasaEngine{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}


	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags){

		_screenHeight = screenHeight;
		_screenWidth = screenWidth;

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;
		if (currentFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (currentFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		_SDLwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);
		if (_SDLwindow == nullptr)
			fatalError("SDL Window could not be created!");

		SDL_GLContext glContext = SDL_GL_CreateContext(_SDLwindow);
		if (glContext == nullptr)
			fatalError("SDL_GL context could not be created!!");

		GLenum error = glewInit(); //Initializing glew
		if (error != GLEW_OK)
			fatalError("Could not initialize glew!");

		printf("***** OpenGL Version : %s *****\n", glGetString(GL_VERSION)); //To print out the version of OpenGL for debugging.


		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		SDL_GL_SetSwapInterval(0); //Set VSYNC

		//Enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0; /// No error
	}


	void Window::swapBuffer(){
		SDL_GL_SwapWindow(_SDLwindow);
	}

}