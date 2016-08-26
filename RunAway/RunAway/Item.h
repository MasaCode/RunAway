#pragma once

#include <string>

#include <MasaEngine/SpriteBatch.h>
#include <MasaEngine/GLTexture.h>
#include <MasaEngine/DebugRenderer.h>

#include "Defenisions.h"

class Item {
public:
	Item() {
		//Empty
	}

	virtual ~Item(){
		//Empty
	}
	
	virtual void init(const glm::vec2& position) = 0;
	virtual void destroy() = 0;

	void draw(MasaEngine::SpriteBatch& _spriteBatch) {
		glm::vec4 uvRect = glm::uvec4(0.0f,0.0f,1.0f,1.0f);
		glm::vec4 destRect = glm::vec4(i_position.x, i_position.y, i_size.x, i_size.y);

		if (i_startDisappear) {
			i_alpha -= 20;
			if (i_alpha <= 0) {
				i_alpha = 0;
				i_isDisappeared = true;
			}
		}

		i_color = MasaEngine::Color(255, 255, 255, i_alpha);

		_spriteBatch.draw(destRect, uvRect,i_texture.id, 0.0f, i_color);
	}

	void drawDebug(MasaEngine::DebugRenderer& debuger) {
		glm::vec4 destRect;
		destRect.x = i_position.x;
		destRect.y = i_position.y;
		destRect.z = i_size.x;
		destRect.w = i_size.y;

		debuger.drawBox(destRect, MasaEngine::Color(0, 0, 255, 255), 0);
	}
	
	glm::vec2 getSize() { return i_size; }
	glm::vec2& getPosition() { return i_position; }
	bool isDisappeared() { return i_isDisappeared; }
	bool isTaken() { return i_startDisappear; }
	ItemInfo getItemInfo() { return i_info; }

	//For Items
	int getEffects() { return i_effects; }
	int getEffectiveTime() { return i_effectiveTime; }
	ItemType getItemType() { return i_type; }

	//For Weapons
	std::string getAttackPath() { return i_attackTexturePath; }
	std::string getSoundPath() { return i_soundPath; }
	int getAttackPoint() { return i_attack; }

protected:
	glm::vec2 i_size = glm::vec2(ITEM_SIZE);
	int i_alpha = 255;
	bool i_isDisappeared = false;
	bool i_startDisappear = false;
	ItemInfo i_info = ItemInfo::ITEM;
	glm::vec2 i_position = glm::vec2(0.0f);
	MasaEngine::GLTexture i_texture;
	MasaEngine::Color i_color = MasaEngine::Color(255, 255, 255, 255);

	//For Item
	int i_effects = 0;
	int i_effectiveTime = 0;
	ItemType i_type = ItemType::HP;

	//For Weapon
	std::string i_attackTexturePath;
	std::string i_soundPath;
	int i_attack = 0;

};
