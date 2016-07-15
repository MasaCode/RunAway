#pragma once

#include <string>
#include "GLTexture.h"

namespace MasaEngine{

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}