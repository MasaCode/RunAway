#include "Potion.h"

#include <MasaEngine/ResourceManager.h>


/////////////////////////////////////////////////////////////////// BIG POTION ////////////////////////////////////////////////////////////////
BigPotion::BigPotion()
{
}

BigPotion::~BigPotion()
{
}

void BigPotion::init(const glm::vec2& position) {

	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/portion.png");


	i_position = position;
	i_type = ItemType::HP;
	i_effects = 100;

}


void BigPotion::destroy() {
	i_startDisappear = true;
}

/////////////////////////////////////////////////////////////////// UNTIL HERE ////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////// SMALL POTION ////////////////////////////////////////////////////////////////
SmallPotion::SmallPotion() {
	//Empty
}

SmallPotion::~SmallPotion() {
	//Empty
}

void SmallPotion::init(const glm::vec2& position) {
	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/portionStick.png");


	i_position = position;
	i_type = ItemType::HP;
	i_effects = 20;
}

void SmallPotion::destroy() {
	i_startDisappear = true;
}

/////////////////////////////////////////////////////////////////// UNTIL HERE ////////////////////////////////////////////////////////////////