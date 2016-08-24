#include "Wolf.h"
#include <random>
#include <ctime>

#include "Player.h"


Wolf::Wolf()
{
	//m_size = 40;
	m_size = glm::vec2(40.0f);
	m_baseHitPoint = 20;
	m_baseAttaciPoint = 5;
	m_experiencePoint = 10;
	m_direction = MovingState::LEFT;
	m_state = MonsterState::NOMAL;
}


Wolf::~Wolf()
{
	//Empty
}

void Wolf::init(int level, float speed,const glm::vec2& position, int movementCount){
	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randLuckyNum(0, 20);

	MasaEngine::GLTexture texture = MasaEngine::ResourceManager::getTexture("Assets/Monsters/wolf.png");

	m_position = position;
	m_speed = speed;
	m_level = level;

	m_attackPoint = m_baseAttaciPoint + level * 3;
	m_hitPoint = m_baseHitPoint + level * 3;
	m_currentHitPoint = m_baseHitPoint + level * 3;


	m_experiencePoint = int(m_experiencePoint * (level / 2));

	m_movementCount = movementCount;


	_luckyNum = (randLuckyNum(randomEngine) + movementCount) % 20;

	//Deciding movement pattern.
	//pattern(levelData);

	//std::cout << "Lucky Num : " <<  _luckyNum << std::endl;

	m_texture.init(texture, glm::ivec2(4, 4));


	m_hpBarTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");
	m_hpTexture = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");

	m_hpPercentage = 1.0f;
	m_hpBarSize = 20;
	m_hpGreen = 255;
	m_hpColor = MasaEngine::Color(0, m_hpGreen, 0, 255);
	m_state = MonsterState::NOMAL;

}

void Wolf::destroy(){
	//Do nothing for now
	Player::getInstance()->getExperiencePoint(m_experiencePoint);
}


glm::vec4 Wolf::animation(){
	glm::vec4 uvRect;
	int tileIndex;
	const int NUM_TILEs = 4;
	float animSpeed = 0.2f;

	
	//Check the MovingState.
	switch (m_direction){
		case MovingState::LEFT:
			tileIndex = 8;
			break;
		case MovingState::RIGHT:
			tileIndex = 4;
			break;
		case MovingState::UP:
			tileIndex = 0;
			break;
		case MovingState::DOWN:
			tileIndex = 12;
			break;
	}

	if (m_currentHitPoint != 0){
		//Increment animation time
		m_animTime += animSpeed * _multiplyer;

		//Apply animation
		tileIndex = tileIndex + (int)m_animTime % NUM_TILEs;
	}
	else{
		m_monsterAlpha -= 10;
		if (m_monsterAlpha < 0){
			m_monsterAlpha = 0;
			m_isDied = true;
		}
	}

	//Get the uv coordinates from the tile index
	uvRect = m_texture.getUVs(tileIndex);

	return uvRect;
}

void Wolf::update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition){
	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randDirection(0, 3);
	std::uniform_int_distribution<int> randDirection2(0, 80);
	std::uniform_int_distribution<int> randChoice(0, 20);
	int tempDirection;
	float distanceX = playerPosition.x - m_position.x;

	if (m_currentHitPoint == 0)
		return;

	if (randChoice(randomEngine) == _luckyNum){
		m_currentMovementIndex = 0;
		_multiplyer = 2.0f;
		m_direction = (MovingState)((randDirection2(randomEngine)) % 4);
	}

	switch (m_direction){
	case MovingState::UP:
		m_position.y += m_speed * _multiplyer;
		break;
	case MovingState::LEFT:
		m_position.x -= m_speed * _multiplyer;
		break;
	case MovingState::DOWN:
		m_position.y -= m_speed * _multiplyer;
		break;
	case MovingState::RIGHT:
		m_position.x += m_speed * _multiplyer;
		break;
	}

	m_currentMovementIndex++;
	collideWithLevel(levelData);

	if (m_currentMovementIndex == m_movementCount || m_isCollided){
		m_currentMovementIndex = 0;
		_multiplyer = 1.0f;
		if (m_isCollided){
			m_isCollided = false;
		}
		
		if (m_state == MonsterState::NOMAL){
			tempDirection = ((randDirection2(randomEngine) * m_movementCount) % 80);
			if ((tempDirection < 10) || (tempDirection >= 40 && tempDirection < 50)){
				m_direction = MovingState::UP;
			}
			else if((tempDirection >= 10 && tempDirection < 20) || (tempDirection >= 50 && tempDirection < 60)){
				m_direction = MovingState::RIGHT;
			}
			else if ((tempDirection >= 20 && tempDirection < 30) || (tempDirection >= 60 && tempDirection < 70)){
				m_direction = MovingState::DOWN;
			}
			else{ //30 ~ 40 or 70 ~ 80
				m_direction = MovingState::LEFT;
			}
		}
	}

}

//void Wolf::pattern(const std::vector<std::string>& levelData){
//	int count = 0;
//	std::mt19937 randomEngine(time(NULL));
//	std::uniform_int_distribution<int> vertical(0, 1);
//	std::uniform_int_distribution<int> horizontal(2, 3);
//	glm::vec2 tempPos = m_position;
//	MovingState verticalMovement = (MovingState)vertical(randomEngine);
//	MovingState horizontalMovement = (MovingState)horizontal(randomEngine);
//
//	int width = levelData[0].size();
//	int height = levelData.size();
//	int deltaX=0, deltaY=0;
//
//
//	glm::vec2 tempPos = m_position;
//
//
//	_pattern.resize(m_movementCount * 2);
//
//	// TODO : need to think about how to decide the movement pattern.
//	//		  Now I'm thinking just store the movement directions and move following it.
//
//	while (count < m_movementCount){
//		int temp = vertical(randomEngine);
//		int x = 0, y = 0;
//		MovingState tempState;
//
//		if (temp == 0){
//			//vertical movement
//			if (verticalMovement == MovingState::DOWN){
//				deltaY = m_speed;
//			}else{
//				deltaY = -m_speed;
//			}
//			tempState = verticalMovement;
//		}
//		else{
//			//horizontal movement
//			if (horizontalMovement == MovingState::LEFT){
//				deltaX = -m_speed;
//			}else{
//				deltaX = m_speed;
//			}
//			tempState = horizontalMovement;
//		}
//
//		x = floor((tempPos.x + deltaX) / (float)TILE_WIDTH);
//		y = floor((tempPos.y + deltaY) / (float)TILE_WIDTH);
//		if (x < width && x >= 0 && y < height && y >= 0){
//			if (levelData[x][y] == '.'){
//				_pattern[count] = tempState;
//				count++;
//				tempPos.x += deltaX;
//				tempPos.y += deltaY;
//			}
//
//		}
//
//		deltaX = 0;
//		deltaY = 0;
//	}
//
//	for (int i = m_movementCount-1; i >= 0; i--){
//		if (_pattern[i] == MovingState::UP){
//			_pattern[count] = MovingState::DOWN;
//		}else if (_pattern[i] == MovingState::LEFT){
//			_pattern[count] = MovingState::RIGHT;
//		}else if (_pattern[i] == MovingState::DOWN){
//			_pattern[count] = MovingState::UP;
//		}else{
//			_pattern[count] = MovingState::LEFT;
//		}
//		count++;
//	}
//
//}

