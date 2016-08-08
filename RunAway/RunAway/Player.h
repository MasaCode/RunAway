#pragma once



#include <MasaEngine/InputManager.h>
#include <MasaEngine/AudioEngine.h>
#include <MasaEngine/SpriteFont.h>

#include "Monster.h"

class Player
{
public:

	~Player();

	static Player* getInstance();

	void init(glm::vec2& position, float speed);

	glm::vec4 animation();

	void draw(MasaEngine::SpriteBatch& _spriteBatch);

	void update(MasaEngine::InputManager& inputManager, const std::vector<std::string>& levelData);

	glm::vec2 calculatePositionInGrid() const;

	void Recreate(const glm::vec2& pos);

	bool takeDamage(int damage);

	bool collideWithLevel(const std::vector<std::string>& levelData);

	void collideWithMonsters(std::vector<Monster*>& monsters);

	void collideWithItems(std::vector<Item*>& items);

	bool isAttackingFast(){ return (_animTime > 4.0f && _playerState == PlayerState::ATTACKING); }

	bool isPlayerDead(){ return _isDead; };

	bool isReachedGoal() { return _isReachedGoal; }

	
	bool isStartedDeathAnimation(){ return _startDeathAnimation; }

	void getExperiencePoint(int exp);

	/*glm::vec2 getPosition(){ return _position; }*/
	glm::vec2 getPosition() const { return _position + glm::vec2(_substructWidth,_substructHeight); }
	glm::vec2 getCutSize() const { return glm::vec2(_substructWidth, _substructHeight); }
	glm::vec2 getSize() const { return _size; }

	void setPosition(const glm::vec2& pos) { _position = pos; }
	void setReachedState(bool isReached) { _isReachedGoal = isReached; }

private:
	//This is singleton design pattern.
	Player();

	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);
	void collideWithTile(glm::vec2 tilePos,bool isRight = false);
	bool collideWithMonster(Monster* monster);
	bool collideWithItem(Item* item);
	void calculateHitPoint();
	bool Attack(const glm::vec2& distance);

private:
	static Player* _instance;

	bool _isStanding = false;

	glm::vec2 _position;
	glm::vec2 _direction = glm::vec2(1.0f, 0.0);
	float _speed = 0;
	float _health = 0;
	float _animTime = 0.0f;

	MasaEngine::Color _color;
	MovingState _state;
	PlayerState _playerState = PlayerState::NOMAL;
	int _multiplyer = 1;
	const int MAX_RUN_COUNT = 30;
	int _runCount = 0;
	bool _couldRun = true;
	bool _finishAttacking = true;
	bool _startDeathAnimation = false;
	bool _isDead = false;
	bool _isLookingItem = false;
	bool _isReachedGoal = false;
	int _counter = 0;

	MasaEngine::TileSheet _texture;
	MasaEngine::TileSheet _attackTexture;
	MasaEngine::GLTexture _hpBar;
	MasaEngine::GLTexture _hitPoint;
	MasaEngine::Color _hpColor;
	float _baseHitPoint = 100.0f;
	float _maxHitPoint = 100.0f;
	float _currentHitPoint = 100.0f;
	float _hpPercentage = 1.0f;
	float _experiencePoint = 0;
	float _needExperiencePoint = 0;
	float _hpSize = 0;
	int _hpGreen = 255;
	int _playerAlpha = 255;

	int _level = 1;
	int _baseAttackPoint = 5;
	int _currentAttackPoint = 5;

	//int _size = 0;
	glm::vec2 _size = glm::vec2(0.0f);

	int _additionalWidth = 0;
	int _additionalHeight = 0;

	int _substructWidth = 0;
	int _substructHeight = 0;

	int _cutSize = 0;

	int _numTiles = 0;

	//For item
	int _effectiveAttackTime = 0;
	int _effectiveSpeedTime = 0;
	int _attackEffectsCount = 0;
	int _speedEffectsCount = 0;
	int _additionalAttack = 0;
	int _additionalSpeed = 0;

	bool _startAttackEffect = false;
	bool _startSpeedEffect = false;

	MasaEngine::AudioEngine _audioEngine;

	MasaEngine::SoundEffect _levelUp;
	MasaEngine::SoundEffect _item;
	MasaEngine::SoundEffect _sword;

	MasaEngine::SpriteFont _font;

};

