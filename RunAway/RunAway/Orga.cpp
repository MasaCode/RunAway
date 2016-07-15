#include "Orga.h"
#include <random>
#include <ctime>
#include "Player.h"

Orga::Orga()
{
	m_baseHitPoint = 25;
	m_baseAttaciPoint = 10;
	m_experiencePoint = 20;
	//m_attackingArea = PLAYER_DIMS + 5;
	m_attackingAreaX = PLAYER_DIM_X + 25;
	m_attackingAreaY = PLAYER_DIM_Y + 15;
	m_followingArea = 200;
	m_direction = MovingState::LEFT;
	m_state = MonsterState::NOMAL;


	m_size = glm::vec2(25, 32);
	m_additionalDims = glm::vec2(32.0f, 12.0f);
	m_substructDims = glm::vec2(16.0f, 6.0f);

	m_hpBarTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");
	m_hpTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");

	m_hpPercentage = 1.0f;
	m_hpBarSize = 20;
	m_hpGreen = 255;
	m_hpColor = MasaEngine::Color(0, m_hpGreen, 0, 255);

	m_waitingTime = 20;

}


Orga::~Orga()
{

}

void Orga::init(int level, float speed, const glm::vec2& position, int movementCount){
	MasaEngine::GLTexture texture = MasaEngine::ResourceManager::getTexture("Assets/Monsters/Orga.png");
	MasaEngine::GLTexture attack = MasaEngine::ResourceManager::getTexture("Assets/Monsters/Orga_AttackingMotion.png");

	m_position = position;
	m_speed = speed;
	m_level = level;
	m_movementCount = movementCount;
	m_followingArea *= (m_level/2);

	m_attackPoint = m_baseAttaciPoint + level * 3;
	m_hitPoint = m_baseHitPoint + level * 3;
	m_currentHitPoint = m_hitPoint;

	m_experiencePoint = int(m_experiencePoint * (level / 2));

	m_texture.init(texture, glm::ivec2(4,5));
	m_attackMotion.init(attack, glm::ivec2(3, 5));


}

glm::vec4 Orga::animation(){
	glm::vec4 uvRect;
	int tileIndex;
	int numTiles = 4;
	float animSpeed = 0.2f;

	if (!m_startDeathAnimation){
		if (m_state == MonsterState::NOMAL || m_state == MonsterState::ATTACKING){

			if (_isChangingState && m_animTime < 2){
				numTiles = 2;
				tileIndex = 18;
				_multiplyer = 0.5f;
			}
			else{
				_isChangingState = false;
				numTiles = m_state == MonsterState::NOMAL ? 4 : 3;

				//Check the MovingState.
				switch (m_direction){
				case MovingState::LEFT:
					if (m_state == MonsterState::ATTACKING)
						tileIndex = 6;
					if (m_state == MonsterState::NOMAL)
						tileIndex = 8;
					break;
				case MovingState::RIGHT:
					if (m_state == MonsterState::ATTACKING)
						tileIndex = 0;
					if (m_state == MonsterState::NOMAL)
						tileIndex = 0;
					break;
				case MovingState::UP:
					if (m_state == MonsterState::NOMAL)
						tileIndex = 4;
					if (m_state == MonsterState::ATTACKING)
						tileIndex = 3;
					break;
				case MovingState::DOWN:
					if (m_state == MonsterState::NOMAL)
						tileIndex = 12;
					if (m_state == MonsterState::ATTACKING)
						tileIndex = 9;
					break;
				}
			}
		}
		else if (m_state == MonsterState::WAITING){
			if (_isChangingState && m_animTime < 1.0f){
				numTiles = 2;
				tileIndex = 16;
			}
			else{
				_isChangingState = false;
				numTiles = 1;
				tileIndex = 17;
			}
		}
	

		//Increment animation time
		m_animTime += animSpeed * _multiplyer;

		//Apply animation
		tileIndex = tileIndex + (int)m_animTime % numTiles;
		
	
		//Get the uv coordinates from the tile index
		if (m_state == MonsterState::ATTACKING){
			uvRect = m_attackMotion.getUVs(tileIndex);
		}
		else{
			uvRect = m_texture.getUVs(tileIndex);
		}

		//Check for attack end
		if (m_animTime > numTiles && m_state == MonsterState::ATTACKING){
			/*if (abs(_directionX) > (PLAYER_DIMS + 10) || abs(_directionY) > (PLAYER_DIMS + 10)){
				m_state = MonsterState::NOMAL;
				}*/
			if (abs(_directionX) > (PLAYER_DIM_X + 10) || abs(_directionY) > (PLAYER_DIM_Y + 10)){
				m_state = MonsterState::NOMAL;
			}
			m_animTime = 0.0f;
			m_attacked = false;
			m_waitingCounter = 0;
		}
	}
	else{
		numTiles = 2;
		tileIndex = 12;
		m_animTime += animSpeed * 0.3f;
		if (m_animTime >= numTiles){
			tileIndex = 13;
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
	}

	return uvRect;
}


void Orga::destroy(){
	m_startDeathAnimation = true;
	m_animTime = 0.0f;
	m_monsterAlpha = 255;
	m_isAttacked = false;
	Player::getInstance()->getExperiencePoint(m_experiencePoint);
 }
 
void Orga::update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition){

	if (m_startDeathAnimation)
		return;

	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randDirection(0, 3);
	glm::vec2 distance = playerPosition - (m_position + m_substructDims);

	//checking direction.
	_directionX = distance.x;
	_directionY = distance.y;

	if (m_state == MonsterState::NOMAL && !_isChangingState && m_waitingCounter == 0 && !m_isAttacked){
		switch (m_direction){
			case MovingState::UP:
				m_position.y += m_speed;
				break;
			case MovingState::RIGHT:
				m_position.x += m_speed;
				break;
			case MovingState::DOWN:
				m_position.y -= m_speed;
				break;
			case MovingState::LEFT:
				m_position.x -= m_speed;
		}
	}




	if (abs(distance.x) <= (m_attackingAreaX) && abs(distance.y) <= (m_attackingAreaY) && !m_isAttacked){
		if (m_waitingCounter >= m_waitingTime){
			if (m_state != MonsterState::ATTACKING){
				m_animTime = 0;
				_multiplyer = 0.6f;
				changeDirection();
			}
			m_state = MonsterState::ATTACKING;

			//Player Attack First.
			if (Player::getInstance()->isAttackingFast() && m_animTime < 3.0f){
				m_state = MonsterState::NOMAL;
				return;
			}

			if (!m_attacked && m_animTime >= 1.0f){
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
	else if (abs(distance.x) < m_followingArea && abs(distance.y) < m_followingArea){
		m_waitingCounter = 0;
		if (m_state != MonsterState::NOMAL){
			m_animTime = 0;
			_isChangingState = true;
		}

		_multiplyer = 1.0f;


		m_state = MonsterState::NOMAL;
		if (m_currentMovementIndex == m_movementCount){
			m_currentMovementIndex = 0;
			changeDirection();
		}


		if (m_isCollided){
			m_isCollided = false;
			//m_direction = (MovingState)randDirection(randomEngine);
		}
		
		m_currentMovementIndex++;
	}
	else{
		m_waitingCounter = 0;
		if (m_state != MonsterState::WAITING){
			_isChangingState = true;
			m_animTime = 0;
			_multiplyer = 0.5f;
		}
		m_state = MonsterState::WAITING;
	}

	collideWithLevel(levelData);

 }


 void Orga::changeDirection(){
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
