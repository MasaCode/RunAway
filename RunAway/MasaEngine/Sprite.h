#pragma once

#include <glew/glew.h>
#include "GLTexture.h"
#include <string>

namespace MasaEngine{

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float height, float width, std::string texturePath);
		void draw();

	private:
		float _x, _y;
		float _height, _width;
		GLuint _vboID; // it's same as unsigned int but the difference is GLuint is guarranteed to be 32 bit.
		GLTexture _texture;

	};

}