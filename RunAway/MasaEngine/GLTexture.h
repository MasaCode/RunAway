#pragma once
#include <glew/glew.h>
#include <string>

namespace MasaEngine{

	struct GLTexture{
		std::string filePath = "";
		GLuint id;
		int wdith;
		int height;
	};

}