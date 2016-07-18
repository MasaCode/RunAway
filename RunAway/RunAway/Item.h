#pragma once

#include <string>

#include <MasaEngine/SpriteBatch.h>
#include <MasaEngine/GLTexture.h>

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
		glm::vec4 destRect = glm::vec4(i_position.x, i_position.y, i_size, i_size);

		if (i_startDisappear) {
			i_alpha -= 20;
			if (i_alpha <= 0) {
				i_alpha = 0;
				i_isTaken = true;
			}
		}

		i_color = MasaEngine::Color(255, 255, 255, i_alpha);

		_spriteBatch.draw(destRect, uvRect,i_texture.id, 0.0f, i_color);
	}

	int getEffects() { return i_effects; }
	int getEffectiveTime() { return i_effectiveTime; }
	int getSize() { return i_size; }
	ItemType getItemType() { return i_type; }
	glm::vec2& getPosition() { return i_position; }
	bool isTaken() { return i_isTaken; }

protected:
	int i_size = ITEM_SIZE;
	int i_alpha = 255;
	int i_effects = 0;
	int i_effectiveTime = 0;
	bool i_isTaken = false;
	bool i_startDisappear = false;
	ItemType i_type = ItemType::HP;
	glm::vec2 i_position = glm::vec2(0.0f);
	MasaEngine::GLTexture i_texture;
	MasaEngine::Color i_color = MasaEngine::Color(255, 255, 255, 255);

};
