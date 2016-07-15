#pragma once

#include "TextureCache.h"
#include <glew/glew.h>

#include <string>

namespace MasaEngine{

	class ResourceManager
	{
	public:

		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;
	};

}