#pragma once

#include <glm/glm.hpp>
#include <MasaEngine/TilSheet.h>
#include <MasaEngine/SpriteBatch.h>
#include <MasaEngine/SpriteFont.h>
#include <MasaEngine/ResourceManager.h>

#include <iostream>
#include <algorithm>

#include "Defenisions.h"

#include "Item.h"


#undef min
#undef max



class Monster
{
public:
	Monster(){}

	virtual ~Monster(){}

	virtual glm::vec4 animation() = 0;
	virtual void init(int level, float speed, const glm::vec2& position, int movementCount = 1) = 0;
	virtual void destroy() = 0;
	virtual void update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition) = 0;

	bool takeDamage(int damage){

		if (m_startDeathAnimation){
			return true;
		}

		m_currentHitPoint -= damage;
		if (m_currentHitPoint <= 0){
			// Monster died;
			m_currentHitPoint = 0;
			destroy();
			return true;
		}
		return false;
	}

	void draw(MasaEngine::SpriteBatch& _spriteBatch){
		{
			glm::vec4 uvRect;
			glm::vec4 destRect;
			destRect.x = m_position.x;
			destRect.y = m_position.y;
			destRect.z = (float)m_size.x + m_additionalDims.x;
			destRect.w = (float)m_size.y + m_additionalDims.y;

			uvRect = animation();

			if (m_isAttacked && m_state == MonsterState::NOMAL && !m_startDeathAnimation){

				if (m_flashing){
					m_monsterAlpha = 255;
					if (m_flashingCount % 2 == 0)
						m_flashing = false;
				}
				else{
					m_monsterAlpha = 0;
					if (m_flashingCount % 2 == 0)
						m_flashing = true;
				}
				
				m_flashingCount++;
			}

			if (!m_isAttacked && !m_startDeathAnimation){
				m_monsterAlpha = 255;
			}

			m_color = MasaEngine::Color(m_monsterRed, m_monsterGreen, m_monsterBlue, m_monsterAlpha);

			if ((m_state == MonsterState::NOMAL || m_state == MonsterState::WAITING) && !m_startDeathAnimation){
				_spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 1.0f, m_color);
			}
			else if (m_state == MonsterState::ATTACKING || m_startDeathAnimation){
				_spriteBatch.draw(destRect, uvRect, m_attackMotion.texture.id, 1.0f, m_color);
			}
		}

		{//This is for HitPoint
			m_hpBarSize = m_size.x;

			glm::vec4 hpUVRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			glm::vec4 hpBarDestRect = glm::vec4(m_position.x + m_substructDims.x, m_position.y + m_size.y + 10, m_hpBarSize, 3.0f);

			calculateHitPoint();

			glm::uvec4 hpDestRect;
			hpDestRect.x = (glm::uint)hpBarDestRect.x;
			hpDestRect.y = (glm::uint)hpBarDestRect.y;
			hpDestRect.z = (glm::uint)(m_hpBarSize * m_hpPercentage);
			//hpDestRect.z = m_hpBarSize * 0.5f;
			hpDestRect.w = (glm::uint)3.0f;

			m_hpColor.r = 255 - m_hpGreen;
			m_hpColor.g = m_hpGreen;


			/*std::cout << "HP : " << m_currentHitPoint << " / " << m_hitPoint << std::endl;
			std::cout << "Percentage : " << m_hpPercentage << std::endl;*/


			_spriteBatch.draw(hpBarDestRect, hpUVRect, m_hpBarTexture.id, 2.0f, m_color);
			_spriteBatch.draw(hpDestRect, hpUVRect, m_hpTexture.id, 2.0f, m_hpColor);

		}

	}

	bool collideWithLevel(const std::vector<std::string>& levelData){
		std::vector<glm::vec2> collideTilePosition;
		

		//check the four corners
		//first corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_substructDims.x , m_position.y + m_substructDims.y);
		
		//Second corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_size.x + m_substructDims.x, m_position.y + m_substructDims.y);

		//Third corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_substructDims.x, m_position.y + m_size.y + m_substructDims.y);
		
		//Fourth corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_size.x + m_substructDims.x, m_position.y + m_size.y + m_substructDims.y);
		
		//if it doesn't collide with anything, just return false
		if (collideTilePosition.size() == 0)
			return false;

		//Do the collision
		for (size_t i = 0; i < collideTilePosition.size(); i++){
			collideWithTile(collideTilePosition[i]);
		}

		return true;

	}

	void collideWithMonsters(std::vector<Monster*>& monsters,int currentMonsterIndex){
		for (size_t i = (size_t)currentMonsterIndex + 1; i < monsters.size(); i++){
				collideWithMonster(monsters[i]);
		}
	}

	void collideWithItems(std::vector<Item*>& items) {
		for (size_t i = 0; i < items.size(); i++) {
			collideWithItem(items[i]);
		}
	}

	bool isDead(){ return m_isDied; }
	bool isStartedDeathAnimation(){ return m_startDeathAnimation; }

	int getAttackPoint(){ return m_attackPoint; }
	int getExperiencePoint(){ return m_experiencePoint; }
	float getSpeed(){ return m_speed; }
	glm::vec2& getPosition(){ return m_position; }
	glm::vec2 getSize(){ return m_size; }
	glm::vec2 getSubstructDims(){ return m_substructDims; }
	MonsterState getMonsterState(){ return m_state; }
	bool isAttacked(){ return m_isAttacked; }

	void setPosition(glm::vec2& pos){ m_position = pos; }
	void setAttackState(bool isAttacked){ m_isAttacked = isAttacked; m_flashingCount = 0; }

protected:

	bool collideWithMonster(Monster* monster){

		const float MIN_DISTANCE_X = (m_size.x/2.0f) + (monster->getSize().x/2.0f);
		const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + (monster->getSize().y / 2.0f);

		glm::vec2 centerPosA = m_position + glm::vec2(m_size / 2.0f) + m_substructDims;
		glm::vec2 centerPosB = monster->getPosition() + glm::vec2(monster->getSize() / 2.0f) + monster->getSubstructDims();

		glm::vec2 distVec = centerPosA - centerPosB;

		glm::vec2 MIN_DISTANCE = glm::vec2(MIN_DISTANCE_X, MIN_DISTANCE_Y);

		float xDepth = MIN_DISTANCE.x - abs(distVec.x);
		float yDepth = MIN_DISTANCE.y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0){
			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
				if (distVec.x < 0){
					m_position.x -= (xDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x + (xDepth / 2.0f), monster->getPosition().y));
				}
				else{
					m_position.x += (xDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x - (xDepth / 2.0f), monster->getPosition().y));
				}
			}
			else{
				if (distVec.y < 0){
					m_position.y -= (yDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x, monster->getPosition().y + (yDepth / 2.0f)));
				}
				else{
					m_position.y += (yDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x, monster->getPosition().y - (yDepth / 2.0f)));
				}
			}

			return true;
		}

		return false;
	}

	bool collideWithItem(Item* item) {

		//If it's too much, just delete last part.
		const float MIN_DISTANCE_X = (m_size.x / 2.0f) + (item->getSize()/ 2.0f) - 5.0f;
		const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + (item->getSize() / 2.0f) - 3.0f;

		glm::vec2 centerPosA = m_position + glm::vec2(m_size / 2.0f) + m_substructDims;
		glm::vec2 centerPosB = item->getPosition() + glm::vec2(item->getSize() / 2.0f);

		glm::vec2 distVec = centerPosA - centerPosB;

		glm::vec2 MIN_DISTANCE = glm::vec2(MIN_DISTANCE_X, MIN_DISTANCE_Y);

		float xDepth = MIN_DISTANCE.x - abs(distVec.x);
		float yDepth = MIN_DISTANCE.y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) {
			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
				if (distVec.x < 0) {
					m_position.x -= xDepth;
				}
				else {
					m_position.x += xDepth;
				}
			}
			else {
				if (distVec.y < 0) {
					m_position.y -= yDepth;
				}
				else {
					m_position.y += yDepth;
				}
			}

			return true;
		}

		return false;
	}


	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y){
		glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

		//If human is outside the world, just return.
		if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size()){
			//std::cout << "Human is created outside the world" << std::endl;
			return;
		}

		//std::cout << cornerPos.x << std::endl;
		//std::cout << cornerPos.y << std::endl;

		if (levelData[(int)cornerPos.y][(int)cornerPos.x] != '.'){
			collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
		}

	}

	//AABB collision
	void collideWithTile(glm::vec2 tilePos){

		const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
		const float MIN_DISTANCE_X = m_size.x / 2.0f + TILE_RADIUS;
		const float MIN_DISTANCE_Y = m_size.y / 2.0f + TILE_RADIUS;

		glm::vec2 centerPlayerPosition = m_position + glm::vec2(m_size / 2.0f) + m_substructDims;
		glm::vec2 distVec = centerPlayerPosition - tilePos;


		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//if this is true, we are colliding with tile
		if (xDepth > 0 && yDepth > 0){
			m_isCollided = true;
			if ((std::max(xDepth, 0.0f)) < (std::max(yDepth, 0.0f))){
				if (distVec.x < 0)
					m_position.x -= xDepth;
				else
					m_position.x += xDepth;
			}
			else{
				if (distVec.y < 0)
					m_position.y -= yDepth;
				else
					m_position.y += yDepth;
			}
		}
	}


	void calculateHitPoint(){
		m_hpPercentage = ((float)m_currentHitPoint / (float)m_hitPoint);
		m_hpGreen = (int)(255 * m_hpPercentage);
	}

protected:

	
	int m_baseHitPoint = 1;
	int m_baseAttaciPoint = 1;
	int m_hitPoint = 0;
	int m_attackPoint = 0;
	int m_currentHitPoint = 0;
	int m_level = 1;
	int m_movementCount = 0;
	int m_currentMovementIndex = 0;
	int m_experiencePoint = 0;
	int m_followingArea = 0;
	int m_attackingAreaX = 0;
	int m_attackingAreaY = 0;
	int m_waitingTime = 0;
	int m_waitingCounter = 0;
	int m_hpGreen = 255;
	int m_monsterRed = 255;
	int m_monsterGreen = 255;
	int m_monsterBlue = 255;
	int m_monsterAlpha = 255;
	int m_flashingCount = 0;
	float m_hpPercentage = 1.0f;
	float m_hpBarSize = 0;
	float m_animTime = 0;
	float m_speed = 0;
	bool m_isCollided = false;
	bool m_isAttacked = false;
	bool m_attacked = false;
	bool m_isDied = false;
	bool m_flashing = false;
	bool m_startDeathAnimation = false;
	glm::vec2 m_position = glm::vec2(0.0f);
	MonsterState m_state = MonsterState::NOMAL;
	MovingState m_direction = MovingState::UP;
	MasaEngine::Color m_color = MasaEngine::Color(255,255,255,255);
	MasaEngine::Color m_hpColor = MasaEngine::Color(0, 0, 0, 255);
	MasaEngine::TileSheet m_texture;
	MasaEngine::TileSheet m_attackMotion;
	MasaEngine::GLTexture m_hpBarTexture;
	MasaEngine::GLTexture m_hpTexture;
	
	glm::vec2 m_size = glm::vec2(0.0f);
	glm::vec2 m_additionalDims = glm::vec2(0.0f);
	glm::vec2 m_substructDims = glm::vec2(0.0f);

};

