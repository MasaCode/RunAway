#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace MasaEngine{

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}


	GLTexture TextureCache::getTexture(std::string texturePath){
		auto mit = _textureMap.find(texturePath);//finding texture;
		

		//check if its not in the map
		if (mit == _textureMap.end()){
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			_textureMap.insert(make_pair(texturePath, newTexture));

			return newTexture;
		}

		return mit->second;
	}

}