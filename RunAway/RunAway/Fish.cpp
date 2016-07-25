#include "Fish.h"
#include <MasaEngine/ResourceManager.h>

/////////////////////////////////////////////////////////////////// GOOD FISH ////////////////////////////////////////////////////////////////
GoodFish::GoodFish()
{
	//Empty
}


GoodFish::~GoodFish()
{
	//Empty
}

void GoodFish::init(const glm::vec2& position) {
	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/goodFish.png");


	i_position = position;
	i_type = ItemType::Speed;
	i_effects = 1;
	i_effectiveTime = 1000;
}


void GoodFish::destroy() {
	i_startDisappear = true;
}


/////////////////////////////////////////////////////////////////// Until Here ////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////// BAD FISH /////////////////////////////////////////////////////////////////

BadFish::BadFish() 
{
	//Empty
}


BadFish::~BadFish()
{
	//Empty
}

void BadFish::init(const glm::vec2& position) {
	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/badFish.png");


	i_position = position;
	i_type = ItemType::Speed;
	i_effects = -1;
	i_effectiveTime = 1000;
}



void BadFish::destroy() {
	i_startDisappear = true;
}


/////////////////////////////////////////////////////////////////// Untill Here ////////////////////////////////////////////////////////////////