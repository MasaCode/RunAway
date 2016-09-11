#include "Mace.h"
#include <MasaEngine/ResourceManager.h>


Mace::Mace()
{
	//Empty
}


Mace::~Mace()
{
	//Empty
}

void Mace::init(const glm::vec2& position) {

	i_texture = MasaEngine::ResourceManager::getTexture("Assets/Items/mace.png");
	i_position = position;
	i_attack = 30;
	i_attackTexturePath = "Assets/Player/player_mace.png";
	i_soundPath = "Sound/mace.mp3";
	i_size = glm::vec2(ITEM_SIZE, 40);
	i_attackingArea = glm::vec2(85, 85);

	i_info = ItemInfo::WEAPON;

}

void Mace::destroy() {
	i_startDisappear = true;
}

