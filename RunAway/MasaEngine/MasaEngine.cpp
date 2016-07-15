#include <SDL\SDL.h>
#include <glew\glew.h>
#include "MasaEngine.h"

namespace MasaEngine{

	int init(){
		SDL_Init(SDL_INIT_EVERYTHING); //Initialize SDL

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //double buffer

		return 0;
	}

}