#include "Skelton.h"

#include "Player.h"
#include <ctime>
#include <random>


Skelton::Skelton()
{
	m_baseHitPoint = 300;
	m_baseAttaciPoint = 10;
	m_experiencePoint = 30;
	m_followingArea = 2000;

	m_attackingAreaX = PLAYER_DIM_X + 30;
	m_attackingAreaY = PLAYER_DIM_Y + 20;

	m_size = glm::vec2(30, 50);
	m_additionalDims = glm::vec2(60, 40.0f);
	m_substructDims = glm::vec2(30.0f, 20.0f);

	_nearRange = 500;

}


Skelton::~Skelton()
{
	//Empty
}

void Skelton::init(int level, float speed, const glm::vec2& position, int movementCount) {
	MasaEngine::GLTexture texture = MasaEngine::ResourceManager::getTexture("Assets/Monsters/skelton_movement.png");
	MasaEngine::GLTexture attack = MasaEngine::ResourceManager::getTexture("Assets/Monsters/skelton_attack.png");

	m_level = level;
	m_speed = speed;
	m_position = position;
	m_movementCount = movementCount;
	m_direction = MovingState::DOWN;

	m_attackPoint = m_baseAttaciPoint + m_level * 3;
	m_hitPoint = m_baseHitPoint + m_level * 3;
	m_currentHitPoint = m_hitPoint;

	m_experiencePoint = int(m_experiencePoint * (m_level / 2));

	m_texture.init(texture, glm::ivec2(8, 4));
	m_attackMotion.init(attack, glm::ivec2(8, 5));


	m_hpBarTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");
	m_hpTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");

	m_hpPercentage = 1.0f;
	m_hpBarSize = 20;
	m_hpGreen = 255;
	m_hpColor = MasaEngine::Color(0, m_hpGreen, 0, 255);

	m_waitingTime = 10;

	_baseSpeed = m_speed;
}

glm::vec4 Skelton::animation() {
	glm::vec4 uvRect;
	int tileIndex;
	float animSpeed = 0.2f;
	const int NUM_TILE = 8;
	
	if (!m_startDeathAnimation) {
		if (m_state == MonsterState::NOMAL || m_state == MonsterState::ATTACKING) {
			//Check the MovingState.
			switch (m_direction) {
			case MovingState::LEFT:
				tileIndex = 16;
				break;
			case MovingState::RIGHT:
				tileIndex = 0;
				break;
			case MovingState::UP:
				tileIndex = 24;
				break;
			case MovingState::DOWN:
				tileIndex = 8;
				break;
			}


			//Apply animation
			tileIndex = tileIndex + (int)m_animTime % NUM_TILE;

			//Check for attack end
				if (m_animTime > NUM_TILE && m_state != MonsterState::NOMAL) {
					if (abs(_distanceX) > (PLAYER_DIM_X / 2.0f) || abs(_distanceX) > (PLAYER_DIM_Y / 2.0f)) {
						m_state = MonsterState::NOMAL;
					}
					m_animTime = 0.0f;
					m_attacked = false;
					m_waitingCounter = 0;
				}


			if (m_state == MonsterState::NOMAL) {
				//Increment animation time
				m_animTime += animSpeed;
				//Get the uv coordinates from the tile index
				uvRect = m_texture.getUVs(tileIndex);
			}else if (m_state == MonsterState::ATTACKING) {
			
				m_animTime += animSpeed * 0.85f;
				uvRect = m_attackMotion.getUVs(tileIndex);

			}
			
		}

	
	}
	else {
		//This is death animation.
		int deathTile = 6;
		tileIndex = 32;
		m_animTime += animSpeed * 0.5f;

		if (m_animTime >= deathTile) {
			tileIndex = 37;
			m_monsterAlpha -= 10;
			if (m_monsterAlpha < 0) {
				m_monsterAlpha = 0;
				m_isDied = true;
			}

		}
		else {
			tileIndex = tileIndex + (int)m_animTime % deathTile;
		}

		uvRect = m_attackMotion.getUVs(tileIndex);

	}

	return uvRect;
}

void Skelton::update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition) {
	//Think about something.
	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randDirection(0, 3);
	glm::vec2 distance = playerPosition - (m_position + m_substructDims);

	//If monster is dead, do nothing.
	if (m_startDeathAnimation)
		return;

	//Check the direction.
	_distanceX = distance.x;
	_distanceY = distance.y;

	if (abs(distance.x) <= (m_attackingAreaX) && abs(distance.y) <= (m_attackingAreaY / 1.5f) && !m_isAttacked) {
		if (m_waitingCounter >= m_waitingTime) {
			if (m_state != MonsterState::ATTACKING) {
				m_animTime = 0;
				m_waitingCounter = 0;
				ChangeDirection();
			}

			m_state = MonsterState::ATTACKING;

			//Player Attack First.
			if (Player::getInstance()->isAttackingFast() && m_animTime < 5.0) {
				m_state = MonsterState::NOMAL;
				m_isAttacked = true;
				m_waitingCounter = 0;
				return;
			}

			//Actual attack here.
			if (!m_attacked && m_animTime >= 5.0f) {
				auto player = Player::getInstance();
				if (!player->isStartedDeathAnimation()) {
					player->takeDamage(m_attackPoint);
					m_attacked = true;
				}
				m_waitingCounter = 0;
			}

		}
		else {
			m_waitingCounter++;
		}


	}
	else if (m_state == MonsterState::NOMAL) {
		m_waitingCounter = 0;
		
		if (abs(distance.x) < m_followingArea && abs(distance.y) < m_followingArea) {
			
			//Actualy this is movement command here.
			if (m_direction == MovingState::UP) {
				m_position.y += m_speed;
			}
			else if (m_direction == MovingState::LEFT) {
				m_position.x -= m_speed;
			}
			else if (m_direction == MovingState::DOWN) {
				m_position.y -= m_speed;
			}
			else {
				m_position.x += m_speed;
			}


			m_currentMovementIndex++;
			if (m_currentMovementIndex == m_movementCount || m_isCollided) {
				m_currentMovementIndex = 0;

				if (abs(distance.x) < _nearRange && abs(distance.y) < _nearRange && _attackingCount == _inRangeMovementCount) {
					_attackingCount = 0;
					//THink I want to speed up.
					m_speed = _baseSpeed * 1.2f;
				}
				else if (_attackingCount < _inRangeMovementCount) {
					_attackingCount++;

				}
				else {
					//It's in following range but not in near range.
					//So speed down.
					m_speed = _baseSpeed;
				}


				auto prevState = m_direction;
				ChangeDirection();
				if (prevState == m_direction && m_isCollided) {
					while (prevState == m_direction) {
						m_direction = (MovingState)randDirection(randomEngine);
					}
				}

				if (m_isCollided) {
					m_isCollided = false;
				}
			}
		}

	}




	//Colliding with Level (walls).
	collideWithLevel(levelData);
}



void Skelton::destroy(){
	m_startDeathAnimation = true;
	m_animTime = 0.0f;
	m_monsterAlpha = 255;
	m_isAttacked = false;
	Player::getInstance()->getExperiencePoint(m_experiencePoint);
}



void Skelton::ChangeDirection() {
	if(_distanceX > 0.0f && _distanceY > 0.0f) {
		//right upper
		if (abs(_distanceX) > abs(_distanceY)) {
			m_direction = MovingState::RIGHT;
		}
		else {
			m_direction = MovingState::UP;
		}
	}
	else if (_distanceX > 0.0f && _distanceY <= 0.0f) {
		//right lower
		if (abs(_distanceX) > abs(_distanceY)) {
			m_direction = MovingState::RIGHT;
		}
		else {
			m_direction = MovingState::DOWN;
		}
	}
	else if (_distanceX < 0.0f && _distanceY > 0.0f) {
		//left upper
		if (abs(_distanceX) > abs(_distanceY)) {
			m_direction = MovingState::LEFT;
		}
		else {
			m_direction = MovingState::UP;
		}
	}
	else {
		//left lower
		if (abs(_distanceX) > abs(_distanceY)) {
			m_direction = MovingState::LEFT;
		}
		else {
			m_direction = MovingState::DOWN;
		}
	}
}
