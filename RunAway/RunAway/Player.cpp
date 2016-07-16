#include "Player.h"

#include <SDL/SDL.h>

Player* Player::_instance = nullptr;


Player::Player()
{
	//Empty
}


Player::~Player()
{
	//Empty
}



Player* Player::getInstance(){
	if (_instance == nullptr){
		_instance = new Player();
	}
	return _instance;
}

void Player::init(glm::vec2& position, float speed,MasaEngine::AudioEngine& audioEngine){
	MasaEngine::GLTexture texture = MasaEngine::ResourceManager::getTexture("Assets/player.png");
	MasaEngine::GLTexture attackTexture = MasaEngine::ResourceManager::getTexture("Assets/player_AttackMotion.png");
	_hpBar = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");
	_hitPoint = MasaEngine::ResourceManager::getTexture("Assets/HitPoint/HitPoint.png");

	_texture.init(texture, glm::ivec2(9, 4));
	_attackTexture.init(attackTexture, glm::ivec2(6, 5));

	_position = position;
	_speed = speed;
	_color = MasaEngine::Color(255, 255, 255, 255);
	_state = MovingState::UP;

	_hpPercentage = 1.0f;
	_hpSize = 30;

	_hpColor = MasaEngine::Color(0, 255, 0, 255);

	
	_maxHitPoint = _level * _baseHitPoint;
	_currentHitPoint = _maxHitPoint;
	_currentAttackPoint = _level * _baseAttackPoint;


	_size = glm::vec2(30.0f, 40.0f);
	_additionalWidth = 130;
	_additionalHeight = 56;
	_substructWidth = 65;
	_substructHeight = 28;

	_needExperiencePoint = 30;

	_levelUp = audioEngine.loadSoundEffect("Sound/LevelUp.ogg");

}

glm::vec4 Player::animation(){
	int tileIndex;

	float animSpeed = 0.3f;
	glm::vec4 uvRect;

	if (!_startDeathAnimation){

		_numTiles = _playerState == PlayerState::NOMAL ? 9 : 6;

		if (_animTime > _numTiles){
			_playerState = PlayerState::NOMAL;
			_animTime = 0;
			_numTiles = 9;
			_finishAttacking = true;
		}


		if (_playerState == PlayerState::NOMAL){
			//_numTiles = 9;

			//Check the state.
			if (_state == MovingState::UP){
				tileIndex = 27;
			}
			else if (_state == MovingState::LEFT){
				tileIndex = 18;
			}
			else if (_state == MovingState::DOWN){
				tileIndex = 9;
			}
			else if (_state == MovingState::RIGHT){
				tileIndex = 0;
			}

			if (_isStanding){
				_numTiles = 1;
				_animTime = 0.0f;
			}
		}
		else{
			//Attacking animation here.
			//_numTiles = 6;

			switch (_state){
			case MovingState::UP:
				tileIndex = 18;
				break;
			case MovingState::DOWN:
				tileIndex = 6;
				break;
			case MovingState::RIGHT:
				tileIndex = 12;
				break;
			case MovingState::LEFT:
				tileIndex = 0;
				break;
			}

		}

		//Increment animation time
		_animTime += animSpeed * _multiplyer;
		//Apply animation
		tileIndex = tileIndex + (int)_animTime % _numTiles;



		if (_playerState == PlayerState::NOMAL){
			//Get the uv coordinates from the tile index
			uvRect = _texture.getUVs(tileIndex);
		}
		else{
			//Get the uv coordinates from the tile index
			uvRect = _attackTexture.getUVs(tileIndex);
		}

	}
	else{
		_numTiles = 6;
		tileIndex = 24;
		_animTime += animSpeed * 0.5f;
		if (_animTime >= _numTiles){
			tileIndex = 29;
			_playerAlpha -= 5;
			if (_playerAlpha < 0){
				_playerAlpha = 0;
				//exit(22);
				_isDead = true;
			}
		}
		else{
			tileIndex = tileIndex + (int)_animTime % _numTiles;
		}

		uvRect = _attackTexture.getUVs(tileIndex);
	}


	return uvRect;
}

void Player::draw(MasaEngine::SpriteBatch& _spriteBatch){
	{// This is for player itself.
		glm::vec4 uvRect;

		glm::vec4 destRect;
		destRect.x = _position.x;
		destRect.y = _position.y;
		destRect.z = (float)_size.x + _additionalWidth;
		destRect.w = (float)_size.y + _additionalHeight;

		uvRect = animation();

		_color = MasaEngine::Color(255, 255, 255, _playerAlpha);

		if (_playerState == PlayerState::NOMAL && !_startDeathAnimation){
			_spriteBatch.draw(destRect, uvRect, _texture.texture.id, 0.0f, _color);
		}
		else{
			_spriteBatch.draw(destRect, uvRect, _attackTexture.texture.id, 0.0f, _color);
		}
	}

	{//This is for HitPoint

		glm::vec4 hpUVRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		glm::vec4 hpBarDestRect = glm::vec4(_position.x+65,_position.y+_size.y+35,_hpSize,5.0f);

		calculateHitPoint();

		glm::uvec4 hpDestRect;
		hpDestRect.x = (glm::uint)hpBarDestRect.x;
		hpDestRect.y = (glm::uint)hpBarDestRect.y;
		hpDestRect.z = (glm::uint)(_hpPercentage * _hpSize);
		hpDestRect.w = (glm::uint)5.0f;

		_hpColor.r = 255 - _hpGreen;
		_hpColor.g = _hpGreen;

		_spriteBatch.draw(hpBarDestRect, hpUVRect, _hpBar.id, 1.0f, _color);
		_spriteBatch.draw(hpDestRect, hpUVRect, _hitPoint.id, 1.0f, _hpColor);

	}

}


void Player::update(MasaEngine::InputManager& inputManager, const std::vector<std::string>& levelData){

	if (_startDeathAnimation)
		return;

	if (_playerState == PlayerState::NOMAL){
		if (inputManager.isKeyDown(SDLK_SPACE) && MAX_RUN_COUNT >= _runCount){
			_multiplyer = 2;
			_runCount++;
			_counter = 0;
		}
		else{
			_multiplyer = 1;
			_couldRun = false;
		}

		if (!_couldRun){
			_counter++;
			if (_counter == 100){
				_couldRun = true;
				_runCount = 0;
			}
		}

		if (inputManager.isKeyDown(SDLK_LEFT)){
			_position.x -= _speed * 0.5f * _multiplyer;
			_state = MovingState::LEFT;
			_isStanding = false;
			_playerState = PlayerState::NOMAL;
		}
		else if (inputManager.isKeyDown(SDLK_RIGHT)){
			_position.x += _speed * 0.5f * _multiplyer;
			_state = MovingState::RIGHT;
			_isStanding = false;
			_playerState = PlayerState::NOMAL;
		}
		else if (inputManager.isKeyDown(SDLK_UP)){
			_position.y += _speed * 0.5f * _multiplyer;
			_state = MovingState::UP;
			_isStanding = false;
			_playerState = PlayerState::NOMAL;

		}
		else if (inputManager.isKeyDown(SDLK_DOWN)){
			_position.y -= _speed * 0.5f * _multiplyer;
			_state = MovingState::DOWN;
			_playerState = PlayerState::NOMAL;
			_isStanding = false;
		}
		else{
			_playerState = PlayerState::NOMAL;
			_isStanding = true;
		}

		if (inputManager.isKeyDown(SDLK_RALT)){
			_playerState = PlayerState::ATTACKING;
			_animTime = 0.0f;
		}

	}

	//TODO : i need to think about this.
	// Do the collision check.
	collideWithLevel(levelData);
}

void Player::getExperiencePoint(int exp){
	_experiencePoint += exp;
	if (_experiencePoint >= _needExperiencePoint){
		_experiencePoint -= _needExperiencePoint;
		_level++;
		_needExperiencePoint = (_needExperiencePoint * (_level / 2));
		_maxHitPoint = _level * _baseHitPoint;
		_currentHitPoint = _maxHitPoint;
		_currentAttackPoint = _level * _baseAttackPoint;

		//Add the effects for level up;

		_levelUp.play();
	}


}

bool Player::collideWithLevel(const std::vector<std::string>& levelData){
	std::vector<glm::vec2> collideTilePosition;

	//check the four corners
	//first corner. Left bottom
	checkTilePosition(levelData, collideTilePosition, _position.x + _substructWidth, _position.y + _substructHeight);

	//Second corner. Right bottom
	checkTilePosition(levelData, collideTilePosition, _position.x + _size.x + _substructWidth, _position.y + _substructHeight);

	//Third corner. Left top
	checkTilePosition(levelData, collideTilePosition, _position.x + _substructWidth, _position.y + _size.y + _substructHeight);

	//Fourth corner Right top
	checkTilePosition(levelData, collideTilePosition, _position.x + _size.x + _substructWidth, _position.y + _size.y + _substructHeight);

	//if it doesn't collide with anything, just return false
	if (collideTilePosition.size() == 0)
		return false;

	//Do the collision
	for (size_t i = 0; i < collideTilePosition.size(); i++){
		collideWithTile(collideTilePosition[i]);
	}

	return true;

}


void Player::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y){
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	//If human is outside the world, just return.
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size()){
		//std::cout << "Human is created outside the world" << std::endl;
		return;
	}
	try{
		if (levelData[(int)cornerPos.y][(int)cornerPos.x] != '.'){
			collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
		}
	}
	catch (std::exception e){
		std::cout << e.what() << std::endl;
		return;
	}
}


//AABB collision
void Player::collideWithTile(glm::vec2 tilePos, bool isRight){

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE_X = (_size.x / 2.0f) + TILE_RADIUS;
	const float MIN_DISTANCE_Y = (_size.y / 2.0f) + TILE_RADIUS;


	glm::vec2 centerPlayerPosition = _position + glm::vec2(_size.x / 2.0f,_size.y / 2.0f) + glm::vec2(_substructWidth,_substructHeight);
	glm::vec2 distVec = centerPlayerPosition - tilePos;


	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0){
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
			if (distVec.x < 0)
				_position.x -= (xDepth);
			else
				_position.x += (xDepth);
		}
		else{
			if (distVec.y < 0)
				_position.y -= (yDepth);
			else
				_position.y += (yDepth);
		}
	}

}

void Player::collideWithMonsters(std::vector<Monster*>& monsters){
	for (size_t i = 0; i < monsters.size(); i++){
		collideWithMonster(monsters[i]);
	}
}

bool Player::collideWithMonster(Monster* monster){
	const float MIN_DISTANCE_X = (_size.x / 2.0f) + (monster->getSize().x / 2.0f);
	const float MIN_DISTANCE_Y = (_size.y / 2.0f) + (monster->getSize().y / 2.0f);

	glm::vec2 centerPosA = _position + glm::vec2(_size / 2.0f) + glm::vec2(_substructWidth,_substructHeight);
	glm::vec2 centerPosB = monster->getPosition() + (monster->getSize()/2.0f) + monster->getSubstructDims();

	glm::vec2 distVec = centerPosA - centerPosB;

	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if (!monster->isStartedDeathAnimation()){
		if (_playerState == PlayerState::ATTACKING && _animTime > 2.0f){
			if (Attack(distVec) && !monster->isAttacked()){
				monster->takeDamage(_currentAttackPoint);
				monster->setAttackState(true);
			}
		}

		if (_animTime > _numTiles){
			monster->setAttackState(false);
		}

		if (xDepth > 0 && yDepth > 0){
			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
				if (distVec.x < 0){
					_position.x -= (xDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x + (xDepth / 2.0f), monster->getPosition().y));
				}
				else{
					_position.x += (xDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x - (xDepth / 2.0f), monster->getPosition().y));
				}
			}
			else{
				if (distVec.y < 0){
					_position.y -= (yDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x, monster->getPosition().y + (yDepth / 2.0f)));
				}
				else{
					_position.y += (yDepth / 2.0f);
					monster->setPosition(glm::vec2(monster->getPosition().x, monster->getPosition().y - (yDepth / 2.0f)));
				}
			}

		}
	}
	else{
		if (xDepth > 0 && yDepth > 0){
			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
				if (distVec.x < 0){
					_position.x -= (xDepth);	
				}else{
					_position.x += (xDepth);
				}
			}
			else{
				if (distVec.y < 0){
					_position.y -= (yDepth);
				}else{
					_position.y += (yDepth);
				}
			}

		}
	}

	
	
	return false;
}

bool Player::Attack(const glm::vec2& distance){


	if (_substructWidth >= abs(distance.x) && _substructHeight*1.5f >= abs(distance.y)){
		if (distance.x < 0.0f && _state == MovingState::RIGHT){
			return true;
		}
		
		if (distance.x >= 0.0f && _state == MovingState::LEFT){
			return true;
		}

		if (distance.y < 0.0f && _state == MovingState::UP){
			return true;
		}

		if (distance.y >= 0.0f && _state == MovingState::DOWN){
			return true;
		}

	}

	return false;
}


void Player::calculateHitPoint(){
	_hpPercentage = _currentHitPoint / _maxHitPoint;
	_hpGreen = (int)(255 * _hpPercentage);
}


bool Player::takeDamage(int damage){
	_currentHitPoint -= damage;
	if (_currentHitPoint < 0){
		_currentHitPoint = 0;
		_startDeathAnimation = true;
		_animTime = 0;
		return false;
	}

	return true;
}


void Player::Recreate(const glm::vec2& pos){
	_playerAlpha = 255;
	_startDeathAnimation = false;
	_isDead = false;

	_position = pos;
	_currentHitPoint = _maxHitPoint;
	_hpGreen = 255;

}