#include "Meat.h"

#include <MasaEngine/ResourceManager.h>


/////////////////////////////////////////////////////////////////// GOOD MEAT ////////////////////////////////////////////////////////////////
GoodMeat::GoodMeat()
{
}

GoodMeat::~GoodMeat()
{
}

void GoodMeat::init(const glm::vec2& position) {

	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/goodMeat.png");


	i_position = position;
	i_type = ItemType::Attack;
	i_effects = 10;
	i_effectiveTime = 1000;

}


void GoodMeat::destroy() {
	i_startDisappear = true;
}

/////////////////////////////////////////////////////////////////// UNTIL HERE ////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////// BAD MEAT ////////////////////////////////////////////////////////////////
BadMeat::BadMeat()
{
}

BadMeat::~BadMeat()
{
}

void BadMeat::init(const glm::vec2& position) {

	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/badMeat.png");


	i_position = position;
	i_type = ItemType::Attack;
	i_effects = -10;
	i_effectiveTime = 1000;

}


void BadMeat::destroy() {
	i_startDisappear = true;
}

/////////////////////////////////////////////////////////////////// UNTIL HERE ////////////////////////////////////////////////////////////////