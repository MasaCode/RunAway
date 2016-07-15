#pragma once

#include <SDL\SDL.h>
#include <glew\glew.h>
#include <string>

namespace MasaEngine{

	enum WindowFlag{ INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };


	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		int getScreenWidth(){ return _screenWidth; }
		int getScreenHeight(){ return _screenHeight; }
		void swapBuffer();
	private:
		SDL_Window *_SDLwindow;
		int _screenWidth = 0, _screenHeight = 0;
	};

}