#include "Sword.h"

#include <MasaEngine/ResourceManager.h>

Sword::Sword()
{

}


Sword::~Sword()
{

}

void Sword::init(const glm::vec2& position) {

	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/sword.png");
	i_position = position;
	i_attack = 20;
	i_attackTexturePath = "Assets/Player/player_sword.png";
	i_soundPath = "Sound/sword.ogg";
	i_size = glm::vec2(ITEM_SIZE, 40);
	i_attackingArea = glm::vec2(160, 100);

	i_info = ItemInfo::WEAPON;

}

void Sword::destroy() {
	i_startDisappear = true;
}