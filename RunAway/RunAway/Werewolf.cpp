#include "Werewolf.h"

#include <random>
#include <ctime>

#include "Player.h"


Werewolf::Werewolf()
{

	m_baseHitPoint = 25;
	m_baseAttaciPoint = 6;
	m_experiencePoint = 12;
	m_followingArea = 500;
	//m_attackingArea = PLAYER_DIMS + 5;
	m_attackingAreaX = PLAYER_DIM_X + 25;
	m_attackingAreaY = PLAYER_DIM_Y + 15;

	m_size = glm::vec2(45, 38);
	m_additionalDims = glm::vec2(20.0f, 0.0f);
	m_substructDims = glm::vec2(10.0f, 2.0f);


}


Werewolf::~Werewolf()
{
}

void Werewolf::init(int level, float speed, const glm::vec2& position, int movementCount){
	MasaEngine::GLTexture texture = MasaEngine::ResourceManager::getTexture("Assets/Monsters/werewolf.png");
	MasaEngine::GLTexture attack = MasaEngine::ResourceManager::getTexture("Assets/Monsters/AttackMotion_Wolf.png");

	m_level = level;
	m_speed = speed;
	m_position = position;
	m_movementCount = movementCount;
	m_direction = MovingState::DOWN;

	m_attackPoint = m_baseAttaciPoint + m_level * 3;
	m_hitPoint = m_baseHitPoint + m_level * 3;
	m_currentHitPoint = m_hitPoint;

	m_experiencePoint = int(m_experiencePoint * (m_level / 2));

	m_texture.init(texture, glm::ivec2(4, 4));
	m_attackMotion.init(attack, glm::ivec2(3, 4));

	m_hpBarTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");
	m_hpTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");

	m_hpPercentage = 1.0f;
	m_hpBarSize = 20;
	m_hpGreen = 255;
	m_hpColor = MasaEngine::Color(0, m_hpGreen, 0, 255);

	m_waitingTime = 10;

}

glm::vec4 Werewolf::animation(){
	glm::vec4 uvRect;
	int tileIndex;
	int numTiles = 0;
	float animSpeed = 0.2f;

	if (!m_startDeathAnimation){
		if (m_state == MonsterState::NOMAL){
			//Check the MovingState.
			switch (m_direction){
			case MovingState::LEFT:
				tileIndex = 12;
				numTiles = 4;
				break;
			case MovingState::RIGHT:
				tileIndex = 4;
				numTiles = 4;
				break;
			case MovingState::UP:
				tileIndex = 0;
				numTiles = 4;
				break;
			case MovingState::DOWN:
				tileIndex = 8;
				numTiles = 4;
				break;
			}

			//Increment animation time
			m_animTime += animSpeed;

			//Apply animation
			tileIndex = tileIndex + (int)m_animTime % numTiles;

			//Get the uv coordinates from the tile index
			uvRect = m_texture.getUVs(tileIndex);
		}
		else if (m_state == MonsterState::ATTACKING){
			tileIndex = 0;
			numTiles = 6;

			m_animTime += animSpeed * 0.75f;
			tileIndex = tileIndex + (int)m_animTime % numTiles;
			uvRect = m_attackMotion.getUVs(tileIndex);

		}


		//Check for attack end
		if (m_animTime > numTiles && m_state != MonsterState::NOMAL){
			if (abs(_directionX) > (PLAYER_DIM_X) || abs(_directionY) > (PLAYER_DIM_Y / 2.0f)){
				m_state = MonsterState::NOMAL;
			}
			m_animTime = 0.0f;
			m_attacked = false;
			m_waitingCounter = 0;
		}

		//Check direction
		if (_directionX < 0.0f && m_state != MonsterState::NOMAL){
			//Direction is left
			uvRect.x += 1.0f / m_attackMotion.dims.x;
			uvRect.z *= -1;
		}
	}
	else{
		numTiles = 5;
		tileIndex = 6;
		m_animTime += animSpeed * 0.5f;
	
		if (m_animTime >= numTiles){
			tileIndex = 10;
			m_monsterAlpha -= 10;
			if (m_monsterAlpha < 0){
				m_monsterAlpha = 0;
				m_isDied = true;
			}
		}
		else{
			tileIndex = tileIndex + (int)m_animTime % numTiles;
		}
	
		uvRect = m_attackMotion.getUVs(tileIndex);

		if (m_direction == MovingState::LEFT){
			//Direction is left
			uvRect.x += 1.0f / m_attackMotion.dims.x;
			uvRect.z *= -1;
		}
	}


	return uvRect;


}



void Werewolf::update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition){
	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randDirection(0, 3);
	glm::vec2 distance = playerPosition - (m_position + m_substructDims);
	

	if (m_startDeathAnimation)
		return;

	//checking direction.
	_directionX = distance.x;
	_directionY = distance.y;
	
	if (abs(distance.x) <= (m_attackingAreaX) && abs(distance.y) <= (m_attackingAreaY / 1.5f) && !m_isAttacked){
		if (m_waitingCounter >= m_waitingTime){
			if (m_state != MonsterState::ATTACKING){
				m_animTime = 0;
				ChangeDirection();
			}
			m_state = MonsterState::ATTACKING;

			//Player Attack First.
			if (Player::getInstance()->isAttackingFast() && m_animTime < 3.0f){
				m_state = MonsterState::NOMAL;
				m_isAttacked = true;
				return;
			}


			if (!m_attacked && m_animTime >= 3.0f){
				auto player = Player::getInstance();
				if (!player->isStartedDeathAnimation()){
					player->takeDamage(m_attackPoint);
					m_attacked = true;
				}
			}
		}
		else{
			m_waitingCounter++;
		}
	}
	else if(m_state == MonsterState::NOMAL){
		m_waitingCounter = 0;


		if (m_direction == MovingState::UP){
			m_position.y += m_speed;
		}
		else if (m_direction == MovingState::LEFT){
			m_position.x -= m_speed;
		}
		else if (m_direction == MovingState::DOWN){
			m_position.y -= m_speed;
		}
		else{
			m_position.x += m_speed;
		}

		m_currentMovementIndex++;
		if (m_currentMovementIndex == m_movementCount || m_isCollided){
			m_currentMovementIndex = 0;
			if (abs(distance.x) < m_followingArea && abs(distance.y) < m_followingArea && _attackingCount == MAX_ATTACKING_MOVEMENT){
				_attackingCount = 0;
				ChangeDirection();
			}
			else if (_attackingCount < MAX_ATTACKING_MOVEMENT){
				_attackingCount++;
			}
			else{
				m_direction = (MovingState)randDirection(randomEngine);
			}

			if (m_isCollided){
				m_isCollided = false;
			}
		}
	}

	collideWithLevel(levelData);

}

void Werewolf::destroy(){
	m_startDeathAnimation = true;
	m_animTime = 0.0f;
	m_monsterAlpha = 255;
	m_isAttacked = false;
	Player::getInstance()->getExperiencePoint(m_experiencePoint);
}


void Werewolf::ChangeDirection(){
	if (_directionX > 0.0f && _directionY > 0.0f){
		//right upper
		if (abs(_directionX) > abs(_directionY)){
			m_direction = MovingState::RIGHT;
		}
		else{
			m_direction = MovingState::UP;
		}
	}
	else if (_directionX > 0.0f && _directionY <= 0.0f){
		//right lower
		if (abs(_directionX) > abs(_directionY)){
			m_direction = MovingState::RIGHT;
		}
		else{
			m_direction = MovingState::DOWN;
		}
	}
	else if (_directionX < 0.0f && _directionY > 0.0f){
		//left upper
		if (abs(_directionX) > abs(_directionY)){
			m_direction = MovingState::LEFT;
		}
		else{
			m_direction = MovingState::UP;
		}
	}
	else{
		//left lower
		if (abs(_directionX) > abs(_directionY)){
			m_direction = MovingState::LEFT;
		}
		else{
			m_direction = MovingState::DOWN;
		}
	}
}